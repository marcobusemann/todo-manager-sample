#pragma once

#include <QMetaOrm/SessionFactory.h>
#include <QSharedPointer>
#include <QList>

#include <data/person.h>

class PersonRepository
{
public:
    typedef QSharedPointer<PersonRepository> Ptr;

    static Ptr factory(const QMetaOrm::SessionFactory::Ptr &sessionFactory);

public:
    PersonRepository(const QMetaOrm::SessionFactory::Ptr &sessionFactory);

    QList<Person::Ptr> getAll();
    Person::Ptr get(int aId);
    void remove(const Person::Ptr &person);
    void save(Person::Ptr &person);

private:
    QMetaOrm::SessionFactory::Ptr m_sessionFactory;
};
