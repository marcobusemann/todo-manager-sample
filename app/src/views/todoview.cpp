#include "todoview.h"
#include "viewmodels/todoviewmodel.h"
#include "ui_todoview.h"

#include "mvvm/lineeditbinding.h"
#include "mvvm/plaintexteditbinding.h"
#include "mvvm/checkboxbinding.h"
#include "mvvm/datetimeeditbinding.h"
#include "mvvm/comboboxbinding.h"

#include "moderngrids/modelbuilder.h"

TodoView::TodoView(TodoViewModel *viewModel, QWidget *parent)
    : QWidget(parent)
    , m_viewModel(viewModel)
    , ui(new Ui::TodoView)
    , m_firstShow(true)
{
    ui->setupUi(this);
    ui->buttonAddWorker->setDefaultAction(m_viewModel->getAddWorkerAction());

    LineEditBinding::factory(ui->editTitle, m_viewModel->getModel(), "title");
    PlainTextEditBinding::factory(ui->editDescription, m_viewModel->getModel(), "description");
    CheckBoxBinding::factory(ui->editCompleted, m_viewModel->getModel(), "completed");
    DateTimeEditBinding::factory(ui->editEndDate, m_viewModel->getModel(), "endDate");

    auto personDisplayHandler = [](const QModelIndex &index, int) -> QVariant {
        return index.data(Qt::UserRole).value<Person::Ptr>()->getFullName();
    };

    auto ownerModel = ModelBuilder::AModel().of(m_viewModel->getAvailableOwners())
        .withColumns(1).withData().forRole(Qt::DisplayRole).withHandler(personDisplayHandler).build();
    ComboBoxBinding::factory(ui->editOwner, ownerModel, m_viewModel->getModel(), "owner");

    auto workerModel = ModelBuilder::AModel().of(m_viewModel->getAvailableWorkers())
        .withColumns(1).withData().forRole(Qt::DisplayRole).withHandler(personDisplayHandler).build();
    ComboBoxBinding::factory(ui->editNewWorker, workerModel, m_viewModel, "currentNewWorker");

    auto choosenWorkerModel = ModelBuilder::AModel().of(m_viewModel->getWorkers())
        .withColumns(1).withData().forRole(Qt::DisplayRole).withHandler(personDisplayHandler).build();
    ui->editWorkers->setModel(choosenWorkerModel);
}

TodoView::~TodoView()
{
    delete ui;
}

void TodoView::showEvent(QShowEvent *)
{
    if (m_firstShow) {
        m_viewModel->initialize();
        m_viewModel->retranslateUi();
        m_firstShow = false;
    }
}
