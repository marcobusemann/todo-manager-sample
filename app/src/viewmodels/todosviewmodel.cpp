#include "todosviewmodel.h"

#include <viewmodels/todoviewmodel.h>
#include <views/todoview.h>

#include <QSortFilterProxyModel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QAction>
#include <QDialog>

QSharedPointer<TodosViewModel> TodosViewModel::factory(
    const TodoRepository::Ptr &todoRepository,
    const PersonRepository::Ptr &personRepository)
{
    return QSharedPointer<TodosViewModel>(new TodosViewModel(todoRepository, personRepository));
}

TodosViewModel::TodosViewModel(
        const TodoRepository::Ptr &todoRepository,
        const PersonRepository::Ptr &personRepository,
        QObject *parent)
    : QObject(parent)
    , m_todoRepository(todoRepository)
    , m_personRepository(personRepository)
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
}

void TodosViewModel::initialize()
{
    m_todos = QObservableList<Todo::Ptr>::fromList(m_todoRepository->getAll());
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
    m_todos.append(newTodo);
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
       m_selectedTodos.append(QPersistentModelIndex(index));
    }

    m_actionEdit->setEnabled(m_selectedTodos.size() == 1);
    m_actionRemove->setEnabled(m_selectedTodos.size() > 0);
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
    auto item = index.data(Qt::UserRole).value<Todo::Ptr>();

    QDialog dialog;

    auto todoViewModel = TodoViewModel::factory(m_personRepository);
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
        m_todos.update(index.row(), todo);
        m_todoRepository->save(todo);
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
        auto item = todoIndex.data(Qt::UserRole).value<Todo::Ptr>();
        m_todoRepository->remove(item);
        m_todos.removeAt(row);
    }
}
