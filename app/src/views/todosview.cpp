#include "todosview.h"
#include "viewmodels/todosviewmodel.h"
#include "ui_todosview.h"

TodosView::TodosView(TodosViewModel *viewModel, QWidget *parent)
    : QWidget(parent)
    , m_viewModel(viewModel)
    , ui(new Ui::TodosView)
    , m_firstShow(true)
{
    ui->setupUi(this);
    connect(ui->todosItemView, &QTreeView::doubleClicked, m_viewModel->getActionEdit(), &QAction::trigger);

    connect(m_viewModel, &TodosViewModel::itemModelChanged, this, &TodosView::updateItemModel);
    connect(ui->editTodoTitle, &QLineEdit::textChanged, m_viewModel, &TodosViewModel::setNewTodoTitle);
    connect(m_viewModel, &TodosViewModel::newTodoTitleChanged, ui->editTodoTitle, &QLineEdit::setText);

    connect(ui->editFind, &QLineEdit::textChanged, m_viewModel, &TodosViewModel::setFilter);

    connect(ui->editTodoTitle, &QLineEdit::returnPressed, m_viewModel->getActionAdd(), &QAction::trigger);

    ui->todosItemView->addAction(m_viewModel->getActionEdit());
    ui->todosItemView->addAction(m_viewModel->getActionRemove());
}

TodosView::~TodosView()
{
    delete ui;
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
    ui->todosItemView->setModel(model);
    connect(ui->todosItemView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &TodosView::updateSelection);
}

void TodosView::updateSelection()
{
    auto indexes = ui->todosItemView->selectionModel()->selectedRows();
    m_viewModel->updateSelection(indexes);
}
