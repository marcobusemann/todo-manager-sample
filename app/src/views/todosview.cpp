#include "todosview.h"
#include "viewmodels/todosviewmodel.h"
#include "ui_todosview.h"

TodosView::TodosView(
    const QSharedPointer<TodosViewModel> &viewModel, 
    QWidget *parent)
    : QWidget(parent)
    , m_viewModel(viewModel)
    , m_ui(new Ui::TodosView())
    , m_firstShow(true)
{
    m_ui->setupUi(this);

    connect(m_ui->todosItemView, &QTreeView::doubleClicked, m_viewModel->getActionEdit(), &QAction::trigger);

    connect(m_viewModel.data(), &TodosViewModel::itemModelChanged, this, &TodosView::updateItemModel);
    connect(m_ui->editTodoTitle, &QLineEdit::textChanged, m_viewModel.data(), &TodosViewModel::setNewTodoTitle);
    connect(m_viewModel.data(), &TodosViewModel::newTodoTitleChanged, m_ui->editTodoTitle, &QLineEdit::setText);

    connect(m_ui->editFind, &QLineEdit::textChanged, m_viewModel.data(), &TodosViewModel::setFilter);

    connect(m_ui->editTodoTitle, &QLineEdit::returnPressed, m_viewModel->getActionAdd(), &QAction::trigger);

    m_ui->todosItemView->addAction(m_viewModel->getActionEdit());
    m_ui->todosItemView->addAction(m_viewModel->getActionRemove());
}

void TodosView::showEvent(QShowEvent *)
{
    if (m_firstShow) {
        m_viewModel->initialize();
        m_firstShow = false;
    }
}

void TodosView::updateItemModel(QAbstractItemModel *model)
{
    m_ui->todosItemView->setModel(model);
    connect(m_ui->todosItemView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &TodosView::updateSelection);
}

void TodosView::updateSelection()
{
    auto indexes = m_ui->todosItemView->selectionModel()->selectedRows();
    m_viewModel->updateSelection(indexes);
}
