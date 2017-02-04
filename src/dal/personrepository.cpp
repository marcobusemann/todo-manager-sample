#include "personrepository.h"

PersonRepository::Ptr PersonRepository::factory(const QMetaOrm::SessionFactory::Ptr &sessionFactory)
{
    return Ptr(new PersonRepository(sessionFactory));
}

PersonRepository::PersonRepository(const QMetaOrm::SessionFactory::Ptr &sessionFactory)
    : m_sessionFactory(sessionFactory)
{
}

QList<Person::Ptr> PersonRepository::getAll()
{
    auto session = m_sessionFactory->createSession();
    return session->selectMany<Person>();
}

Person::Ptr PersonRepository::get(int aId)
{
    auto session = m_sessionFactory->createSession();
    return session->selectOne<Person>(aId);
}

void PersonRepository::remove(const Person::Ptr &person)
{
    auto session = m_sessionFactory->createSession();
    session->remove(person);
    session->commit();
}

void PersonRepository::save(Person::Ptr &person)
{
    auto session = m_sessionFactory->createSession();
    session->save(person);
    session->commit();
}
