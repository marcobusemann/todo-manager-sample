#pragma once

#include <dal/personrepository.h>

#include <data/todo.h>
#include <data/person.h>

#include <moderngrids/observablelist.h>

#include <QObject>
#include <QList>

class QAction;

class PersonListModel;

class TodoViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Person::Ptr currentNewWorker READ getCurrentNewWorker WRITE setCurrentNewWorker NOTIFY currentNewWorkerChanged)

public:
    TodoViewModel(const PersonRepository::Ptr &personRepository, QObject *parent = nullptr);
    void retranslateUi();
    void initialize();

    void setTodo(const Todo::Ptr &todo);
    Todo::Ptr buildTodo() const;
    Todo::Ptr getModel() const;

    const ObservableList<Person::Ptr>::Ptr &getAvailableOwners() const;
    const ObservableList<Person::Ptr>::Ptr &getAvailableWorkers() const;
    const ObservableList<Person::Ptr>::Ptr &getWorkers() const;
    const Person::Ptr &getCurrentNewWorker() const;

    QAction *getAddWorkerAction() const;

public slots:
    void setCurrentNewWorker(const Person::Ptr &aCurrentNewWorker);

private:
    PersonRepository::Ptr m_personRepository;

    QAction *m_addWorkerAction;

    Todo::Ptr m_todo;

    ObservableList<Person::Ptr>::Ptr m_allPersons;
    ObservableList<Person::Ptr>::Ptr m_availableWorkers;
    ObservableList<Person::Ptr>::Ptr m_workers;
    Person::Ptr m_currentNewWorker;

    ObservableList<Person::Ptr>::Ptr filterPersons(const ObservableList<Person::Ptr>::Ptr &source, const QList<Person::Ptr> &itemsToSubstract);
    void setAvailableWorkers(const ObservableList<Person::Ptr>::Ptr &workers);
    void setAvailableOwners(const ObservableList<Person::Ptr>::Ptr &owners);

private slots:
    void addNewWorker(const Person::Ptr &newPerson);
    void addNewWorkerByAction();
    void updateAvailableWorkers();

signals:
    void availableWorkersChanged(const QList<Person::Ptr> &workers);
    void currentNewWorkerChanged(const Person::Ptr &currentNewPerson);
    void modelChanged();
};
