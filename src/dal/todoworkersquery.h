#pragma once

#include <QMetaOrm/SessionFactory.h>
#include <QSharedPointer>
#include <QList>

#include <data/person.h>
#include <data/todo.h>

class TodoWorkersQuery
{
public:
    typedef QSharedPointer<TodoWorkersQuery> Ptr;

    static Ptr factory(const QMetaOrm::SessionFactory::Ptr &sessionFactory);

public:
    TodoWorkersQuery(const QMetaOrm::SessionFactory::Ptr &sessionFactory);
    QList<Person::Ptr> getWorkersFor(const Todo::Ptr &todo);

private:
    QMetaOrm::SessionFactory::Ptr m_sessionFactory;
};
