#include "viewmodels/todoviewmodel.h"
#include "views/todoview.h"

#include "ui_todoview.h"

#include "mvvm/lineeditbinding.h"
#include "mvvm/plaintexteditbinding.h"
#include "mvvm/checkboxbinding.h"
#include "mvvm/datetimeeditbinding.h"
#include "mvvm/comboboxbinding.h"

#include <QModernGrids/Builders/QmgModelBuilder.h>

TodoView::TodoView(
    const QSharedPointer<TodoViewModel> &viewModel,
    QWidget *parent)
    : QWidget(parent)
    , m_viewModel(viewModel)
    , m_ui(new Ui::TodoView())
    , m_firstShow(true)
{
    m_ui->setupUi(this);
    m_ui->buttonAddWorker->setDefaultAction(m_viewModel->getAddWorkerAction());

    LineEditBinding::factory(m_ui->editTitle, m_viewModel->getModel(), "title");
    PlainTextEditBinding::factory(m_ui->editDescription, m_viewModel->getModel(), "description");
    CheckBoxBinding::factory(m_ui->editCompleted, m_viewModel->getModel(), "completed");
    DateTimeEditBinding::factory(m_ui->editEndDate, m_viewModel->getModel(), "endDate");

    auto personDisplayHandler = [](const QModelIndex &index, int) -> QVariant {
        return index.data(Qt::UserRole).value<Person::Ptr>()->getFullName();
    };

    auto ownerModel = QmgModelBuilder::AListModelFor(m_viewModel->getAvailableOwners(), this)
        .withData(Qt::DisplayRole, personDisplayHandler)
        .build();
    ComboBoxBinding::factory(m_ui->editOwner, ownerModel, m_viewModel->getModel(), "owner");

    auto workerModel = QmgModelBuilder::AListModelFor(m_viewModel->getAvailableWorkers(), this)
        .withData(Qt::DisplayRole, personDisplayHandler)
        .build();
    ComboBoxBinding::factory(m_ui->editNewWorker, workerModel, m_viewModel, "currentNewWorker");

    auto choosenWorkerModel = QmgModelBuilder::AListModelFor(m_viewModel->getWorkers(), this)
        .withData(Qt::DisplayRole, personDisplayHandler)
        .build();
    m_ui->editWorkers->setModel(choosenWorkerModel);
}

void TodoView::showEvent(QShowEvent *)
{
    if (m_firstShow) {
        m_viewModel->initialize();
        m_viewModel->retranslateUi();
        m_firstShow = false;
    }
}
