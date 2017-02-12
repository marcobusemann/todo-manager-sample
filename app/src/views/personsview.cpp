#include "personsview.h"
#include "ui_personsview.h"

#include <viewmodels/personsviewmodel.h>

PersonsView::PersonsView(PersonsViewModel *viewModel, QWidget *parent)
    : QWidget(parent)
    , m_viewModel(viewModel)
    , ui(new Ui::PersonsView)
    , m_firstShow(true)
{
    ui->setupUi(this);
    connect(ui->personsItemView, &QTreeView::doubleClicked, m_viewModel->getActionEdit(), &QAction::trigger);

    connect(m_viewModel, &PersonsViewModel::itemModelChanged, this, &PersonsView::updateItemModel);
    connect(ui->editNewPersonName, &QLineEdit::textChanged, m_viewModel, &PersonsViewModel::setNewPersonNames);
    connect(m_viewModel, &PersonsViewModel::newPersonNamesChanged, ui->editNewPersonName, &QLineEdit::setText);

    connect(ui->editSearch, &QLineEdit::textChanged, m_viewModel, &PersonsViewModel::setFilter);

    connect(ui->editNewPersonName, &QLineEdit::returnPressed, m_viewModel->getActionAdd(), &QAction::trigger);

    ui->personsItemView->addAction(m_viewModel->getActionEdit());
    ui->personsItemView->addAction(m_viewModel->getActionRemove());
}

PersonsView::~PersonsView()
{
    delete ui;
}

void PersonsView::showEvent(QShowEvent *)
{
    if (m_firstShow) {
        m_viewModel->initialize();
        m_firstShow = false;
    }
}

void PersonsView::updateItemModel(QAbstractItemModel *model)
{
    ui->personsItemView->setModel(model);
    connect(ui->personsItemView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &PersonsView::updateSelection);
}

void PersonsView::updateSelection()
{
    auto indexes = ui->personsItemView->selectionModel()->selectedRows();
    m_viewModel->updateSelection(indexes);
}
