#include "todorepository.h"
#include "todoworkerrepository.h"
#include "todomapping.h"

TodoRepository::Ptr TodoRepository::factory(const QMetaOrm::SessionFactory::Ptr &sessionFactory)
{
    return Ptr(new TodoRepository(sessionFactory));
}

TodoRepository::TodoRepository(const QMetaOrm::SessionFactory::Ptr &sessionFactory)
    : m_sessionFactory(sessionFactory)
    , m_workerQuery(TodoWorkersQuery::factory(sessionFactory))
{
}

QList<Todo::Ptr> TodoRepository::getAll()
{
    auto session = m_sessionFactory->createSession();
    QList<Todo::Ptr> result;
    session->selectManyByCallback<Todo>([&](const Todo::Ptr &todo) -> bool {
        if (todo != nullptr) {
            auto workers = m_workerQuery->getWorkersFor(todo);
            todo->setWorkers(workers);
        }
        result.append(todo);
        return true;
    });
    return result;
}

Todo::Ptr TodoRepository::get(int aId)
{
    auto session = m_sessionFactory->createSession();
    auto todo = session->selectOne<Todo>(aId);
    if (todo != nullptr) {
        auto workers = m_workerQuery->getWorkersFor(todo);
        todo->setWorkers(workers);
    }
    return todo;
}

void TodoRepository::remove(const Todo::Ptr &todo)
{
    auto session = m_sessionFactory->createSession();
    session->remove<Todo>(todo);
    session->commit();
}

void TodoRepository::save(Todo::Ptr &todo)
{
    auto workerRepository = TodoWorkersRepository::factory(m_sessionFactory);
    workerRepository->remove(todo, todo->getWorkers());

    auto session = m_sessionFactory->createSession();
    session->save<Todo>(todo);
    session->commit();

    workerRepository->save(todo);
}
