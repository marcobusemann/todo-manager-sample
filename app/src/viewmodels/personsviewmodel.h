#pragma once

#include <QSharedPointer>

#include <dal/personrepository.h>
#include <data/person.h>

#include <moderngrids/qobservablelist.h>

class QAction;

class PersonsViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString newPersonName READ getNewPersonName WRITE setNewPersonName NOTIFY newPersonNameChanged)

public:
    static QSharedPointer<PersonsViewModel> factory(const PersonRepository::Ptr &personRepository);

public:
    PersonsViewModel(const PersonRepository::Ptr &personRepository, QObject *parent = nullptr);
    void initialize();

    QObservableList<Person::Ptr> &getPersons();

    QAction *getActionAdd() const;
    QAction *getActionEdit() const;
    QAction *getActionRemove() const;

    const QString &getNewPersonName() const;

public slots:
    void addPerson(const QString &names);
    void setNewPersonName(const QString &name);
    void updateSelection(const QList<QPersistentModelIndex> &selectedItems);

private:
    PersonRepository::Ptr m_personRepository;

    QList<QPersistentModelIndex> m_selectedPersons;
    QObservableList<Person::Ptr> m_persons;
    QString m_newPersonName;

    QAction *m_actionAdd;
    QAction *m_actionEdit;
    QAction *m_actionRemove;

private slots:
    void addByAction();
    void editByAction();
    void removeByAction();

signals:
    void newPersonNameChanged(const QString &newPersonName);
};
