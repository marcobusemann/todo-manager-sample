#include "todoview.h"
#include "viewmodels/todoviewmodel.h"
#include "ui_todoview.h"

TodoView::TodoView(TodoViewModel *viewModel, QWidget *parent)
    : QWidget(parent)
    , m_viewModel(viewModel)
    , ui(new Ui::TodoView)
    , m_firstShow(true)
{
    ui->setupUi(this);

    connect(ui->editTitle, &QLineEdit::textChanged, m_viewModel, &TodoViewModel::setTitle);
    connect(m_viewModel, &TodoViewModel::titleChanged, ui->editTitle, &QLineEdit::setText);

    connect(ui->editDescription, &QPlainTextEdit::textChanged, this, &TodoView::updateViewModelDescription);
    connect(m_viewModel, &TodoViewModel::descriptionChanged, this, &TodoView::updateViewDescription);

    connect(ui->editEndDate, &QDateTimeEdit::dateTimeChanged, m_viewModel, &TodoViewModel::setEndDate);
    connect(m_viewModel, &TodoViewModel::endDateChanged, ui->editEndDate, &QDateTimeEdit::setDateTime);

    connect(ui->editCompleted, &QCheckBox::toggled, m_viewModel, &TodoViewModel::setCompleted);
    connect(m_viewModel, &TodoViewModel::completionChanged, ui->editCompleted, &QCheckBox::setChecked);

    connect(ui->editOwner, SIGNAL(currentIndexChanged(int)), this, SLOT(updateViewModelOwner()));
    connect(m_viewModel, &TodoViewModel::ownerChanged, this, &TodoView::updateOwner);
    connect(m_viewModel, &TodoViewModel::availableOwnersChanged, this, &TodoView::updateAvailableOwners);

    connect(m_viewModel, &TodoViewModel::availableWorkersChanged, this, &TodoView::updateAvailableWorkers);
    connect(ui->editNewWorker, SIGNAL(currentIndexChanged(int)), this, SLOT(updateViewModelNewWorker()));
    connect(m_viewModel, &TodoViewModel::workersChanged, this, &TodoView::setWorkers);
    connect(m_viewModel, &TodoViewModel::workerAdded, this, &TodoView::addWorker);

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
        m_firstShow = false;
    }
}

void TodoView::updateViewModelDescription()
{
    m_viewModel->setDescription(ui->editDescription->toPlainText());
}

void TodoView::updateViewModelNewWorker()
{
    m_viewModel->setNewWorker(ui->editNewWorker->currentData().value<Person::Ptr>());
}

void TodoView::updateViewDescription(const QString &description)
{
    if (ui->editDescription->toPlainText() != description)
        ui->editDescription->setPlainText(description);
}

void TodoView::updateAvailableOwners(const QList<Person::Ptr> &persons)
{
    ui->editOwner->clear();

    auto currentOwner = m_viewModel->getOwner();

    int currentOwnerIndex = 0;
    for (int i = 0; i < persons.size(); i++) {
        auto person = persons[i];
        if (currentOwner != nullptr && person->getId() == currentOwner->getId())
            currentOwnerIndex = i;
        ui->editOwner->addItem(person->getFullName(), QVariant::fromValue(person));
    }

    ui->editOwner->setCurrentIndex(currentOwnerIndex);
}

void TodoView::updateAvailableWorkers(const QList<Person::Ptr> &persons)
{
    ui->editNewWorker->clear();

    for (int i = 0; i < persons.size(); i++) {
        auto person = persons[i];
        ui->editNewWorker->addItem(person->getFullName(), QVariant::fromValue(person));
    }
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
}

void TodoView::updateViewModelOwner()
{
    auto person = ui->editOwner->currentData().value<Person::Ptr>();
    m_viewModel->setOwner(person);
}

void TodoView::addWorker(const Person::Ptr &person)
{
    auto *item = new QListWidgetItem(person->getFullName());
    item->setData(Qt::UserRole, QVariant::fromValue(person));
    ui->editWorkers->addItem(item);
}

void TodoView::setWorkers(const QList<Person::Ptr> &persons)
{
    ui->editWorkers->clear();
    for (auto person : persons) {
        addWorker(person);
    }
}
