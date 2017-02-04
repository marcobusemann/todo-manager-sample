#pragma once

#include <QMetaOrm/SessionFactory.h>
#include <QSharedPointer>
#include <QList>

#include <dal/todoworkersquery.h>

#include <data/todo.h>

class TodoRepository
{
public:
    typedef QSharedPointer<TodoRepository> Ptr;

    static Ptr factory(const QMetaOrm::SessionFactory::Ptr &sessionFactory);

public:
    TodoRepository(const QMetaOrm::SessionFactory::Ptr &sessionFactory);

    QList<Todo::Ptr> getAll();
    Todo::Ptr get(int aId);
    void remove(const Todo::Ptr &todo);
    void save(Todo::Ptr &todo);

private:
    QMetaOrm::SessionFactory::Ptr m_sessionFactory;
    TodoWorkersQuery::Ptr m_workerQuery;
};
