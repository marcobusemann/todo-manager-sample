#include "todosviewmodel.h"

#include <viewmodels/todoviewmodel.h>
#include <views/todoview.h>

#include <QSortFilterProxyModel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QAction>
#include <QDialog>

TodosViewModel::TodosViewModel(
        const TodoRepository::Ptr &todoRepository,
        const PersonRepository::Ptr &personRepository,
        QObject *parent)
    : QAbstractTableModel(parent)
    , m_todoRepository(todoRepository)
    , m_personRepository(personRepository)
    , m_filterProxyModel(new QSortFilterProxyModel(this))
    , m_actionAdd(new QAction(this))
    , m_actionEdit(new QAction(this))
    , m_actionRemove(new QAction(this))
{
    connect(m_actionAdd, &QAction::triggered, this, &TodosViewModel::addByAction);
    connect(m_actionEdit, &QAction::triggered, this, &TodosViewModel::editByAction);
    connect(m_actionRemove, &QAction::triggered, this, &TodosViewModel::removeByAction);

    m_actionAdd->setText(tr("Add todo"));
    m_actionEdit->setText(tr("Edit todo"));
    m_actionRemove->setText(tr("Remove todo"));

    m_actionEdit->setEnabled(false);
    m_actionRemove->setEnabled(false);

    m_filterProxyModel->setSourceModel(this);
    m_filterProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
}

void TodosViewModel::initialize()
{
    setTodos(m_todoRepository->getAll());
    emit itemModelChanged(m_filterProxyModel);
}

int TodosViewModel::rowCount(const QModelIndex &) const
{
    return m_todos.size();
}

int TodosViewModel::columnCount(const QModelIndex &) const
{
    return Column::Last;
}

QVariant TodosViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto &item = m_todos[index.row()];

    QVariant result;

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case Column::Title:
            result = item->getTitle();
            break;
        case Column::EndDate:
            result = item->getEndDate();
            break;
        }
    }
    else if (role == Qt::ForegroundRole) {
        result = item->isCompleted() ? QColor(Qt::gray) : QColor(Qt::black);
    }

    return result;
}

QVariant TodosViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();

    QVariant result;
    switch (section) {
    case Column::Title:
        result = tr("Title");
        break;
    case Column::EndDate:
        result = tr("End date");
        break;
    }
    return result;
}

QAction *TodosViewModel::getActionAdd() const
{
    return m_actionAdd;
}

QAction *TodosViewModel::getActionEdit() const
{
    return m_actionEdit;
}

QAction *TodosViewModel::getActionRemove() const
{
    return m_actionRemove;
}

void TodosViewModel::addTodo(const QString &title)
{
    Todo::Ptr newTodo(new Todo());
    newTodo->setTitle(title);
    m_todoRepository->save(newTodo);

    beginInsertRows(QModelIndex(), m_todos.size(), m_todos.size());
    m_todos.append(newTodo);
    endInsertRows();
}

void TodosViewModel::setNewTodoTitle(const QString &title)
{
    if (m_newTodoTitle == title) return;
    m_newTodoTitle = title;
    emit newTodoTitleChanged(m_newTodoTitle);
}

void TodosViewModel::updateSelection(const QModelIndexList &selectedIndexes)
{
    m_selectedTodos.clear();
    for (auto index : selectedIndexes) {
        m_selectedTodos.append(QPersistentModelIndex(m_filterProxyModel->mapToSource(index)));
    }

    m_actionEdit->setEnabled(m_selectedTodos.size() == 1);
    m_actionRemove->setEnabled(m_selectedTodos.size() > 0);
}

void TodosViewModel::setFilter(const QString &filter)
{
    m_filterProxyModel->setFilterWildcard(filter);
}

void TodosViewModel::setTodos(const QList<Todo::Ptr> &todos)
{
    beginResetModel();
    m_todos = todos;
    endResetModel();
}

void TodosViewModel::addByAction()
{
    if (!m_newTodoTitle.isEmpty())
        addTodo(m_newTodoTitle);
    setNewTodoTitle(QString());
}

void TodosViewModel::editByAction()
{
    auto index = m_selectedTodos[0];
    auto &item = m_todos[index.row()];

    QDialog dialog;

    auto *todoViewModel = new TodoViewModel(m_personRepository, &dialog);
    auto *todoView = new TodoView(todoViewModel);
    todoViewModel->setTodo(item);

    auto *buttonSave = new QPushButton();
    buttonSave->setText(tr("Save"));
    connect(buttonSave, &QPushButton::clicked, &dialog, &QDialog::accept);

    auto *buttonCancel = new QPushButton();
    buttonCancel->setText(tr("Cancel"));
    connect(buttonCancel, &QPushButton::clicked, &dialog, &QDialog::reject);

    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addItem(new QSpacerItem(-1, -1, QSizePolicy::Expanding));
    buttonLayout->addWidget(buttonSave);
    buttonLayout->addWidget(buttonCancel);

    auto *dialogLayout = new QVBoxLayout();
    dialogLayout->addWidget(todoView);
    dialogLayout->addLayout(buttonLayout);
    dialogLayout->setMargin(0);

    dialog.setLayout(dialogLayout);
    auto execCode = dialog.exec();

    if (execCode == QDialog::Accepted) {
        auto todo = todoViewModel->buildTodo();
        m_todos[index.row()] = todo;
        m_todoRepository->save(todo);
        emit dataChanged(index.sibling(index.row(), 0), index.sibling(index.row(), Column::Last - 1));
    }
}

void TodosViewModel::removeByAction()
{
    if (m_selectedTodos.size() <= 0)
        return;

    auto todoIndexesToRemove = m_selectedTodos;
    updateSelection(QModelIndexList());

    for (auto todoIndex : todoIndexesToRemove) {
        auto row = todoIndex.row();
        beginRemoveRows(QModelIndex(), row, row);
        m_todoRepository->remove(m_todos[row]);
        m_todos.removeAt(row);
        endRemoveRows();
    }
}
