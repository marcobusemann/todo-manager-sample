#pragma once

#include <dal/personrepository.h>

#include <data/todo.h>
#include <data/person.h>

#include <moderngrids/qobservablelist.h>

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

    QObservableList<Person::Ptr> &getAvailableOwners();
    QObservableList<Person::Ptr> &getAvailableWorkers();
    QObservableList<Person::Ptr> &getWorkers();
    const Person::Ptr &getCurrentNewWorker() const;

    QAction *getAddWorkerAction() const;

public slots:
    void setCurrentNewWorker(const Person::Ptr &aCurrentNewWorker);

private:
    PersonRepository::Ptr m_personRepository;

    QAction *m_addWorkerAction;

    Todo::Ptr m_todo;

    QObservableList<Person::Ptr> m_allPersons;
    QObservableList<Person::Ptr> m_availableWorkers;
    QObservableList<Person::Ptr> m_workers;
    Person::Ptr m_currentNewWorker;

    QObservableList<Person::Ptr> filterPersons(const QObservableList<Person::Ptr> &source, const QList<Person::Ptr> &itemsToSubstract);
    void setAvailableWorkers(const QObservableList<Person::Ptr> &workers);
    void setAvailableOwners(const QObservableList<Person::Ptr> &owners);

private slots:
    void addNewWorker(const Person::Ptr &newPerson);
    void addNewWorkerByAction();
    void updateAvailableWorkers();

signals:
    void availableWorkersChanged(const QList<Person::Ptr> &workers);
    void currentNewWorkerChanged(const Person::Ptr &currentNewPerson);
    void modelChanged();
};
