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
    connect(m_todo.data(), &Todo::workersChanged, this, &TodoViewModel::updateAvailableWorkers);
}

void TodoViewModel::retranslateUi()
{
    m_addWorkerAction->setText(tr("Add worker"));
    m_addWorkerAction->setToolTip(tr("Add a new worker"));
}

void TodoViewModel::initialize()
{
    auto persons = m_personRepository->getAll();
    int tmp = persons.size();
    setAvailableOwners(persons);
    setAvailableWorkers(filterPersons(persons, m_todo->getWorkers()));
}

void TodoViewModel::setTodo(const Todo::Ptr &todo)
{
    m_todo->copy(todo);
}

Todo::Ptr TodoViewModel::buildTodo() const
{
    Todo::Ptr result(new Todo());
    result->copy(m_todo);
    return result;
}

Todo::Ptr TodoViewModel::getModel() const
{
    return m_todo;
}

const QList<Person::Ptr> &TodoViewModel::getAvailableOwners() const
{
    return m_allPersons;
}

QAction *TodoViewModel::getAddWorkerAction() const
{
    return m_addWorkerAction;
}

void TodoViewModel::updateAvailableWorkers()
{
    auto workers = m_todo->getWorkers();
    m_workersItemModel->setItems(workers);
    setAvailableWorkers(filterPersons(m_allPersons, workers));
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
