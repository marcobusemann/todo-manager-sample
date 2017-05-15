#include "todosview.h"
#include "viewmodels/todosviewmodel.h"
#include "ui_todosview.h"

#include "mvvm\lineeditbinding.h"

#include "moderngrids\modelbuilder.h"

#include <qsortfilterproxymodel.h>

TodosView::TodosView(
    const QSharedPointer<TodosViewModel> &viewModel, 
    QWidget *parent)
    : QWidget(parent)
    , m_viewModel(viewModel)
    , m_ui(new Ui::TodosView())
    , m_sortFilterItemModel(new QSortFilterProxyModel(this))
    , m_firstShow(true)
{
    m_ui->setupUi(this);

    LineEditBinding::factory(m_ui->editTodoTitle, m_viewModel.data(), "newTodoTitle");

    connect(m_ui->todosItemView, &QTreeView::doubleClicked, m_viewModel->getActionEdit(), &QAction::trigger);
    connect(m_ui->editTodoTitle, &QLineEdit::returnPressed, m_viewModel->getActionAdd(), &QAction::trigger);

    m_ui->todosItemView->addAction(m_viewModel->getActionEdit());
    m_ui->todosItemView->addAction(m_viewModel->getActionRemove());

    auto itemModel = ModelBuilder::AModelFor(m_viewModel->getTodos(), this)
       .withColumns(2)
       .withHorizontalHeaderData([&](int section, int role) -> QVariant {
          QVariant result;

          if (!role == Qt::DisplayRole) 
             return result;

          switch (section) {
          case 0:
             result = tr("Title");
             break;
          case 1:
             result = tr("End date");
             break;
          }
          return result;
       })
       .withData([](const QModelIndex &index, int role, std::function<QVariant()> defaultValue) -> QVariant {
          auto item = index.data(Qt::UserRole).value<Todo::Ptr>();
          QVariant result = defaultValue();

          if (role == Qt::DisplayRole) {
             switch (index.column()) {
             case 0:
                result = item->getTitle();
                break;
             case 1:
                result = item->getEndDate();
                break;
             }
          }
          else if (role == Qt::ForegroundRole) {
             result = item->isCompleted() ? QColor(Qt::gray) : QColor(Qt::black);
          }

          return result;
       }).build();

    m_sortFilterItemModel->setSourceModel(itemModel);
    m_sortFilterItemModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    connect(m_ui->editFind, &QLineEdit::textChanged, m_sortFilterItemModel, &QSortFilterProxyModel::setFilterWildcard);

    m_ui->todosItemView->setModel(m_sortFilterItemModel);
    connect(m_ui->todosItemView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &TodosView::updateSelection);
}

void TodosView::showEvent(QShowEvent *)
{
    if (m_firstShow) {
        m_viewModel->initialize();
        m_firstShow = false;
    }
}

void TodosView::updateSelection()
{
    auto indexes = m_ui->todosItemView->selectionModel()->selectedRows();
    auto mappedIndexes = QList<QModelIndex>();

    for (auto &index : indexes)
       mappedIndexes.append(QPersistentModelIndex(index));

    m_viewModel->updateSelection(mappedIndexes);
}
