#include "todoworkerrepository.h"
#include <QMetaOrm/MetaEntityBuilder.h>

TodoWorkersRepository::Ptr TodoWorkersRepository::factory(const QMetaOrm::SessionFactory::Ptr &sessionFactory)
{
    return Ptr(new TodoWorkersRepository(sessionFactory));
}

TodoWorkersRepository::TodoWorkersRepository(const QMetaOrm::SessionFactory::Ptr &sessionFactory)
    : m_sessionFactory(sessionFactory)
{

}

QList<Person::Ptr> TodoWorkersRepository::getAll(const Todo::Ptr &todo)
{
    const QString SQL = "SELECT p.* FROM person p"
            " JOIN todo_workers tw ON (p.id = tw.id_person)"
            " WHERE tw.id_todo = ?";
    auto session = m_sessionFactory->createSession();
    return session->selectManyBySql<Person>(SQL, QVariantList() << todo->getId());
}

void TodoWorkersRepository::save(const Todo::Ptr &todo)
{
    /*
    static auto mapper = QMetaOrm::MetaEntityBuilder::anEntity()
            .forSource("todo_workers")
            .withData("id_todo", "id_todo")
            .withData("id_person", "id_person")
            .build();

    auto session = m_sessionFactory->createSession();
    auto workers = todo->getWorkers();

    for (auto worker : workers) {
        auto object = QSharedPointer<QObject>(new QObject());
        object->setProperty("id_todo", todo->getId());
        object->setProperty("id_person", worker->getId());
        session->save(object, mapper);
    }
    */
    const QString SQL = "INSERT INTO todo_workers (id_todo, id_person) values (?,?)";

    auto session = m_sessionFactory->createSession();
    auto workers = todo->getWorkers();
    for (auto worker : workers)
        session->saveBySql(SQL, QVariantList() << todo->getId() << worker->getId());
    session->commit();
}

void TodoWorkersRepository::remove(const Todo::Ptr &todo, const QList<Person::Ptr> &workers)
{
    const QString SQL = "delete from todo_workers where id_todo=? and id_person=?";
    auto session = m_sessionFactory->createSession();
    for (auto worker : workers)
        session->removeBySql(SQL, QVariantList() << todo->getId() << worker->getId());
    session->commit();
}
