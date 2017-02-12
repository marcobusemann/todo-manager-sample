#pragma once

#include <dal/personrepository.h>

#include <data/todo.h>
#include <data/person.h>

#include <QObject>
#include <QList>

class QAction;

class PersonListModel;

class TodoViewModel : public QObject
{
    Q_OBJECT

public:
    TodoViewModel(const PersonRepository::Ptr &personRepository, QObject *parent = nullptr);
    void retranslateUi();
    void initialize();

    void setTodo(const Todo::Ptr &todo);
    Todo::Ptr buildTodo() const;

    const Person::Ptr &getOwner() const;
    const QList<Person::Ptr> &getAvailableOwners() const;

    QAction *getAddWorkerAction() const;

public slots:
    void setTitle(const QString &title);
    void setDescription(const QString &description);
    void setEndDate(const QDateTime &endDate);
    void setCompleted(bool isCompleted);
    void setOwner(const Person::Ptr &owner);
    void setWorkers(const QList<Person::Ptr> &workers);
    void setNewWorker(const Person::Ptr &newWorker);

private:
    PersonRepository::Ptr m_personRepository;

    QAction *m_addWorkerAction;

    Todo::Ptr m_todo;
    PersonListModel  *m_workersItemModel;

    QList<Person::Ptr> m_allPersons;
    QList<Person::Ptr> m_availableWorkers;

    Person::Ptr m_newWorker;

    QList<Person::Ptr> filterPersons(const QList<Person::Ptr> &source, const QList<Person::Ptr> &itemsToSubstract);
    void setAvailableWorkers(const QList<Person::Ptr> &workers);
    void setAvailableOwners(const QList<Person::Ptr> &owners);

private slots:
    void addNewWorker(const Person::Ptr &newPerson);
    void addNewWorkerByAction();

signals:
    void titleChanged(const QString &title);
    void descriptionChanged(const QString &description);
    void endDateChanged(const QDateTime &endDate);
    void completionChanged(bool isCompleted);
    void ownerChanged(const Person::Ptr &owner);
    void workersChanged(const QList<Person::Ptr> &workers);
    void newWorkerChanged(const Person::Ptr &newWorker);

    void availableOwnersChanged(const QList<Person::Ptr> &workers);
    void availableWorkersChanged(const QList<Person::Ptr> &workers);

    void workerAdded(const Person::Ptr &person);
};

Q_DECLARE_METATYPE(QList<Person::Ptr>)
