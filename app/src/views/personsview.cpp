#include "personsview.h"
#include "ui_personsview.h"

#include <viewmodels/personsviewmodel.h>

PersonsView::PersonsView(
    const QSharedPointer<PersonsViewModel> &viewModel,
    QWidget *parent)
    : QWidget(parent)
    , m_viewModel(viewModel)
    , m_ui(new Ui::PersonsView())
    , m_firstShow(true)
{
    m_ui->setupUi(this);

    connect(m_ui->personsItemView, &QTreeView::doubleClicked, m_viewModel->getActionEdit(), &QAction::trigger);

    connect(m_viewModel.data(), &PersonsViewModel::itemModelChanged, this, &PersonsView::updateItemModel);
    connect(m_ui->editNewPersonName, &QLineEdit::textChanged, m_viewModel.data(), &PersonsViewModel::setNewPersonNames);
    connect(m_viewModel.data(), &PersonsViewModel::newPersonNamesChanged, m_ui->editNewPersonName, &QLineEdit::setText);

    connect(m_ui->editSearch, &QLineEdit::textChanged, m_viewModel.data(), &PersonsViewModel::setFilter);

    connect(m_ui->editNewPersonName, &QLineEdit::returnPressed, m_viewModel->getActionAdd(), &QAction::trigger);

    m_ui->personsItemView->addAction(m_viewModel->getActionEdit());
    m_ui->personsItemView->addAction(m_viewModel->getActionRemove());
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
    m_ui->personsItemView->setModel(model);
    connect(m_ui->personsItemView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &PersonsView::updateSelection);
}

void PersonsView::updateSelection()
{
    auto indexes = m_ui->personsItemView->selectionModel()->selectedRows();
    m_viewModel->updateSelection(indexes);
}
