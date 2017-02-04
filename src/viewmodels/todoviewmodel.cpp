#include <viewmodels/todoviewmodel.h>
#include <itemmodels/personlistmodel.h>
#include <QAction>

TodoViewModel::TodoViewModel(const PersonRepository::Ptr &personRepository, QObject *parent)
    : QObject(parent)
    , m_personRepository(personRepository)
    , m_addWorkerAction(new QAction(this))
    , m_todo(Todo::Ptr(new Todo()))
    , m_workersItemModel(new PersonListModel(this))
{
    connect(m_addWorkerAction, &QAction::triggered, this, &TodoViewModel::addNewWorkerByAction);
}

void TodoViewModel::retranslateUi()
{
    m_addWorkerAction->setText(tr("Add worker"));
    m_addWorkerAction->setToolTip(tr("Add a new worker"));
}

void TodoViewModel::initialize()
{
    auto persons = m_personRepository->getAll();
    setAvailableOwners(persons);
    setAvailableWorkers(filterPersons(persons, m_todo->getWorkers()));
}

void TodoViewModel::setTodo(const Todo::Ptr &todo)
{
    m_todo->setId(todo->getId());
    setTitle(todo->getTitle());
    setDescription(todo->getDescription());
    setEndDate(todo->getEndDate());
    setCompleted(todo->isCompleted());
    setOwner(todo->getOwner());
    setWorkers(todo->getWorkers());
}

Todo::Ptr TodoViewModel::buildTodo() const
{
    Todo::Ptr result(new Todo());
    result->setId(m_todo->getId());
    result->setTitle(m_todo->getTitle());
    result->setDescription(m_todo->getDescription());
    result->setEndDate(m_todo->getEndDate());
    result->setCompleted(m_todo->isCompleted());
    result->setOwner(m_todo->getOwner());
    result->setWorkers(m_todo->getWorkers());
    return result;
}

const Person::Ptr &TodoViewModel::getOwner() const
{
    return m_todo->getOwner();
}

const QList<Person::Ptr> &TodoViewModel::getAvailableOwners() const
{
    return m_allPersons;
}

QAction *TodoViewModel::getAddWorkerAction() const
{
    return m_addWorkerAction;
}

void TodoViewModel::setTitle(const QString &title)
{
    if (m_todo->getTitle() == title) return;
    m_todo->setTitle(title);
    emit titleChanged(title);
}

void TodoViewModel::setDescription(const QString &description)
{
    if (m_todo->getDescription() == description) return;
    m_todo->setDescription(description);
    emit descriptionChanged(description);
}

void TodoViewModel::setEndDate(const QDateTime &endDate)
{
    if (m_todo->getEndDate() == endDate) return;
    m_todo->setEndDate(endDate);
    emit endDateChanged(endDate);
}

void TodoViewModel::setCompleted(bool isCompleted)
{
    if (m_todo->isCompleted() == isCompleted) return;
    m_todo->setCompleted(isCompleted);
    emit completionChanged(isCompleted);
}

void TodoViewModel::setOwner(const Person::Ptr &owner)
{
    auto currentOwner = m_todo->getOwner();
    if (currentOwner == nullptr && owner == nullptr) return;
    if ((currentOwner == nullptr && owner != nullptr) ||
        (currentOwner != nullptr && owner == nullptr) ||
        (currentOwner->getId() != owner->getId())) {
        m_todo->setOwner(owner);
        emit ownerChanged(owner);
    }
}

void TodoViewModel::setWorkers(const QList<Person::Ptr> &workers)
{
    m_workersItemModel->setItems(workers);
    m_todo->setWorkers(workers);
    setAvailableWorkers(filterPersons(m_allPersons, workers));
    emit workersChanged(workers);
}

void TodoViewModel::setNewWorker(const Person::Ptr &newWorker)
{
    if (newWorker == m_newWorker) return;
    m_newWorker = newWorker;
    emit newWorkerChanged(newWorker);
}

QList<Person::Ptr> TodoViewModel::filterPersons(const QList<Person::Ptr> &source, const QList<Person::Ptr> &itemsToSubstract)
{
    QList<Person::Ptr> result = source;

    for (auto item : itemsToSubstract) {
        auto foundIterator = std::find_if(result.begin(), result.end(), [item](const Person::Ptr &value) -> bool {
            return value->getId() == item->getId();
        });

        if (foundIterator != result.end()) {
            int index = std::distance(result.begin(), foundIterator);
            result.removeAt(index);
        }
    }

    return result;
}

void TodoViewModel::setAvailableWorkers(const QList<Person::Ptr> &workers)
{
    if (m_availableWorkers == workers) return;
    m_availableWorkers = workers;
    emit availableWorkersChanged(workers);
}

void TodoViewModel::setAvailableOwners(const QList<Person::Ptr> &owners)
{
    if (owners == m_allPersons) return;
    m_allPersons = owners;
    emit availableOwnersChanged(owners);
}

void TodoViewModel::addNewWorkerByAction()
{
    if (m_newWorker == nullptr) return;
    addNewWorker(m_newWorker);
}

void TodoViewModel::addNewWorker(const Person::Ptr &newPerson)
{
    auto workers = m_todo->getWorkers();
    workers.append(newPerson);
    m_todo->setWorkers(workers);
    emit workerAdded(newPerson);
    setAvailableWorkers(filterPersons(m_allPersons, workers));
}
