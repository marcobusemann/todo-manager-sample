#include "personsview.h"
#include "ui_personsview.h"

#include <viewmodels/personsviewmodel.h>

#include <moderngrids/modelbuilder.h>
#include "moderngrids/proxymodelutils.h"

#include <mvvm/lineeditbinding.h>

PersonsView::PersonsView(
    const QSharedPointer<PersonsViewModel> &viewModel,
    QWidget *parent)
    : QWidget(parent)
    , m_viewModel(viewModel)
    , m_ui(new Ui::PersonsView())
    , m_firstShow(true)
{
    m_ui->setupUi(this);

    m_model = ModelBuilder::AModelFor(m_viewModel->getPersons(), this)
        .withColumns(2)
        .withData(Qt::DisplayRole, [](const QModelIndex &index, int role) -> QVariant {
            auto person = index.data(Qt::UserRole).value<Person::Ptr>();
            QVariant result;
            if (index.column() == 0)
                result = person->getSurname();
            else if (index.column() == 1)
                result = person->getName();
            return result;
        })
        .withHorizontalHeaderData([](int column, int role) -> QVariant {
            QVariant result;
            if (role == Qt::DisplayRole)
            {
                if (column == 0)
                    result = tr("Surname");
                else if (column == 1)
                    result = tr("Name");
            }
            return result;
        })
        .withSortAndFilter([&](auto model) {
           model->setFilterCaseSensitivity(Qt::CaseInsensitive);
           model->setFilterKeyColumn(-1);
           connect(m_ui->editSearch, &QLineEdit::textChanged, model, &QSortFilterProxyModel::setFilterWildcard);
        })
        .build();

    m_ui->personsItemView->setModel(m_model);

    connect(m_ui->personsItemView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &PersonsView::updateSelection);

    LineEditBinding::factory(m_ui->editNewPersonName, m_viewModel, "newPersonName");

    connect(m_ui->personsItemView, &QTreeView::doubleClicked, m_viewModel->getActionEdit(), &QAction::trigger);
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

void PersonsView::updateSelection()
{
    auto indexes = m_ui->personsItemView->selectionModel()->selectedRows();
    auto items = QList<QPersistentModelIndex>();
    for (auto index : indexes) {
        items.append(ProxyModelUtils::mapToRoot(m_model, index));
    }
    m_viewModel->updateSelection(items);
}
