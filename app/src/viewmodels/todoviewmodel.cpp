#include <viewmodels/todoviewmodel.h>
#include <itemmodels/personlistmodel.h>
#include <QAction>

TodoViewModel::TodoViewModel(const PersonRepository::Ptr &personRepository, QObject *parent)
    : QObject(parent)
    , m_personRepository(personRepository)
    , m_addWorkerAction(new QAction(this))
    , m_todo(Todo::Ptr(new Todo()))
    , m_allPersons(ObservableList<Person::Ptr>::empty())
    , m_availableWorkers(ObservableList<Person::Ptr>::empty())
    , m_workers(ObservableList<Person::Ptr>::empty())
{
    connect(m_addWorkerAction, &QAction::triggered, this, &TodoViewModel::addNewWorkerByAction);

    m_workers->OnChange = [&]() {
        m_todo->setWorkers(m_workers->toList());
        updateAvailableWorkers();
    };
}

void TodoViewModel::retranslateUi()
{
    m_addWorkerAction->setText(tr("Add worker"));
    m_addWorkerAction->setToolTip(tr("Add a new worker"));
}

void TodoViewModel::initialize()
{
    auto persons = m_personRepository->getAll();
    auto observablePersons = ObservableList<Person::Ptr>::fromList(persons);
    setAvailableOwners(observablePersons);
    setAvailableWorkers(filterPersons(observablePersons, m_todo->getWorkers()));
}

void TodoViewModel::setTodo(const Todo::Ptr &todo)
{
    m_todo->copy(todo);
    if (todo != nullptr)
        *m_workers = todo->getWorkers();
    emit modelChanged();
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

const ObservableList<Person::Ptr>::Ptr &TodoViewModel::getAvailableOwners() const
{
    return m_allPersons;
}

const ObservableList<Person::Ptr>::Ptr &TodoViewModel::getAvailableWorkers() const
{
    return m_availableWorkers;
}

const ObservableList<Person::Ptr>::Ptr &TodoViewModel::getWorkers() const
{
    return m_workers;
}

QAction *TodoViewModel::getAddWorkerAction() const
{
    return m_addWorkerAction;
}

void TodoViewModel::updateAvailableWorkers()
{
    auto workers = m_todo->getWorkers();
    setAvailableWorkers(filterPersons(m_allPersons, workers));
}

void TodoViewModel::setCurrentNewWorker(const Person::Ptr &newWorker)
{
    if (newWorker == m_currentNewWorker) return;
    m_currentNewWorker = newWorker;
    emit currentNewWorkerChanged(m_currentNewWorker);
}

const Person::Ptr &TodoViewModel::getCurrentNewWorker() const
{
    return m_currentNewWorker;
}

ObservableList<Person::Ptr>::Ptr TodoViewModel::filterPersons(const ObservableList<Person::Ptr>::Ptr &source, const QList<Person::Ptr> &itemsToSubstract)
{
    auto sourceQtList = source->toList();
    auto result = ObservableList<Person::Ptr>::fromList(source);

    for (auto item : itemsToSubstract) {
        auto foundIterator = std::find_if(sourceQtList.begin(), sourceQtList.end(), [item](const Person::Ptr &value) -> bool {
            return value->getId() == item->getId();
        });

        if (foundIterator != sourceQtList.end()) {
            int index = std::distance(sourceQtList.begin(), foundIterator);
            result->removeAt(index);
            sourceQtList.removeAt(index);
        }
    }

    return result;
}

void TodoViewModel::setAvailableWorkers(const ObservableList<Person::Ptr>::Ptr &workers)
{
    if (m_availableWorkers == workers) return;
    *m_availableWorkers = *workers;
}

void TodoViewModel::setAvailableOwners(const ObservableList<Person::Ptr>::Ptr &owners)
{
    if (m_allPersons == owners) return;
    *m_allPersons = *owners;
}

void TodoViewModel::addNewWorkerByAction()
{
    if (m_currentNewWorker == nullptr) return;
    addNewWorker(m_currentNewWorker);
}

void TodoViewModel::addNewWorker(const Person::Ptr &newPerson)
{
    if (newPerson == nullptr) return;
    m_workers->append(newPerson);
}
