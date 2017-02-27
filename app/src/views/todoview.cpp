#include "todoview.h"
#include "viewmodels/todoviewmodel.h"
#include "ui_todoview.h"
#include "mvvm/lineeditbinding.h"

TodoView::TodoView(TodoViewModel *viewModel, QWidget *parent)
    : QWidget(parent)
    , m_viewModel(viewModel)
    , ui(new Ui::TodoView)
    , m_firstShow(true)
{
    ui->setupUi(this);

    LineEditBinding::factory(ui->editTitle, m_viewModel->getModel(), "title");

    connect(ui->editDescription, &QPlainTextEdit::textChanged, this, &TodoView::updateViewModelDescription);
    connect(m_viewModel->getModel().data(), &Todo::descriptionChanged, this, &TodoView::updateViewDescription);

    connect(ui->editEndDate, &QDateTimeEdit::dateTimeChanged, m_viewModel->getModel().data(), &Todo::setEndDate);
    connect(m_viewModel->getModel().data(), &Todo::endDateChanged, ui->editEndDate, &QDateTimeEdit::setDateTime);

    connect(ui->editCompleted, &QCheckBox::toggled, m_viewModel->getModel().data(), &Todo::setCompleted);
    connect(m_viewModel->getModel().data(), &Todo::completionChanged, ui->editCompleted, &QCheckBox::setChecked);

    connect(ui->editOwner, SIGNAL(currentIndexChanged(int)), this, SLOT(updateViewModelOwner()));
    connect(m_viewModel->getModel().data(), &Todo::ownerChanged, this, &TodoView::updateOwner);
    connect(m_viewModel, &TodoViewModel::availableOwnersChanged, this, &TodoView::updateAvailableOwners);

    connect(m_viewModel, &TodoViewModel::availableWorkersChanged, this, &TodoView::updateAvailableWorkers);
    connect(ui->editNewWorker, SIGNAL(currentIndexChanged(int)), this, SLOT(updateViewModelCurrentNewWorker()));
    connect(m_viewModel->getModel().data(), &Todo::workersChanged, this, &TodoView::setWorkers);

    ui->buttonAddWorker->setDefaultAction(m_viewModel->getAddWorkerAction());
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

void TodoView::updateViewModelDescription()
{
    m_viewModel->getModel()->setDescription(ui->editDescription->toPlainText());
}

void TodoView::updateViewDescription(const QString &description)
{
    if (ui->editDescription->toPlainText() != description)
        ui->editDescription->setPlainText(description);
}

void TodoView::updateAvailableOwners(const QList<Person::Ptr> &persons)
{
    ui->editOwner->clear();

    auto currentOwner = m_viewModel->getModel()->getOwner();

    int currentOwnerIndex = 0;
    ui->editNewWorker->blockSignals(true);
    for (int i = 0; i < persons.size(); i++) {
        auto person = persons[i];
        if (currentOwner != nullptr && person->getId() == currentOwner->getId())
            currentOwnerIndex = i;
        ui->editOwner->addItem(person->getFullName(), QVariant::fromValue(person));
    }
    ui->editNewWorker->blockSignals(false);
    ui->editOwner->setCurrentIndex(currentOwnerIndex);
}

void TodoView::updateAvailableWorkers(const QList<Person::Ptr> &persons)
{
    ui->editNewWorker->clear();
    ui->editNewWorker->blockSignals(true);
    for (int i = 0; i < persons.size(); i++) {
        auto person = persons[i];
        ui->editNewWorker->addItem(person->getFullName(), QVariant::fromValue(person));
    }
    ui->editNewWorker->blockSignals(false);
    ui->editNewWorker->setCurrentIndex(0);
    updateViewModelCurrentNewWorker();
}

void TodoView::updateViewModelCurrentNewWorker()
{
    m_viewModel->setCurrentNewWorker(ui->editNewWorker->currentData().value<Person::Ptr>());
}

void TodoView::updateOwner(const Person::Ptr &person)
{
    auto currentPerson = ui->editOwner->currentData().value<Person::Ptr>();
    if (currentPerson != nullptr && person->getId() == currentPerson->getId()) return;

    auto availableOwners = m_viewModel->getAvailableOwners();
    auto foundIt = std::find_if(availableOwners.begin(), availableOwners.end(), [&](const Person::Ptr &person) {
        return person->getId() == currentPerson->getId();
    });

    if (foundIt != availableOwners.end()) {
       auto index = std::distance(availableOwners.begin(), foundIt);
       ui->editOwner->setCurrentIndex(index);
    }
    else
       ui->editOwner->setCurrentIndex(0);
}

void TodoView::updateViewModelOwner()
{
    auto person = ui->editOwner->currentData().value<Person::Ptr>();
    m_viewModel->getModel()->setOwner(person);
}

void TodoView::setWorkers(const QList<Person::Ptr> &persons)
{
    ui->editWorkers->clear();
    for (auto person : persons) {
        auto *item = new QListWidgetItem(person->getFullName());
        item->setData(Qt::UserRole, QVariant::fromValue(person));
        ui->editWorkers->addItem(item);
    }
}
