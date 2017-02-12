#pragma once

#include <QMetaOrm/SessionFactory.h>
#include <QSharedPointer>
#include <QList>

#include <data/person.h>
#include <data/todo.h>

class TodoWorkersRepository
{
public:
    typedef QSharedPointer<TodoWorkersRepository> Ptr;

    static Ptr factory(const QMetaOrm::SessionFactory::Ptr &sessionFactory);

public:
    TodoWorkersRepository(const QMetaOrm::SessionFactory::Ptr &sessionFactory);
    virtual ~TodoWorkersRepository() {}

    QList<Person::Ptr> getAll(const Todo::Ptr &todo);
    void save(const Todo::Ptr &todo);
    void remove(const Todo::Ptr &todo, const QList<Person::Ptr> &workers);

private:
    QMetaOrm::SessionFactory::Ptr m_sessionFactory;
};
