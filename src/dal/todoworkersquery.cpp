#include "todoworkersquery.h"

TodoWorkersQuery::Ptr TodoWorkersQuery::factory(const QMetaOrm::SessionFactory::Ptr &sessionFactory)
{
    return Ptr(new TodoWorkersQuery(sessionFactory));
}

TodoWorkersQuery::TodoWorkersQuery(const QMetaOrm::SessionFactory::Ptr &sessionFactory)
    : m_sessionFactory(sessionFactory)
{

}

QList<Person::Ptr> TodoWorkersQuery::getWorkersFor(const Todo::Ptr &todo)
{
    const QString SQL = "SELECT p.* FROM persons p JOIN todo_workers tw ON (p.id = tw.id_person) WHERE tw.id_todo = ?";
    auto session = m_sessionFactory->createSession();
    return session->selectManyBySql<Person>(SQL, QVariantList() << todo->getId());
}
