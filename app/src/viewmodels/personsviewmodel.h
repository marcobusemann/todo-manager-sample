#pragma once

#include <QAbstractTableModel>

#include <dal/personrepository.h>
#include <data/person.h>

class QSortFilterProxyModel;
class QAction;

class PersonsViewModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Column {
        Surname = 0,
        Name,
        Last
    };

public:
    PersonsViewModel(const PersonRepository::Ptr &personRepository, QObject *parent = nullptr);
    void initialize();

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    QAction *getActionAdd() const;
    QAction *getActionEdit() const;
    QAction *getActionRemove() const;

public slots:
    void addPerson(const QString &names);
    void setNewPersonNames(const QString &names);
    void updateSelection(const QModelIndexList &selectedIndexes);
    void setFilter(const QString &filter);

private:
    void setPersons(const QList<Person::Ptr> &persons);

    PersonRepository::Ptr m_personRepository;
    QSortFilterProxyModel *m_filterProxyModel;

    QList<QPersistentModelIndex> m_selectedPersons;
    QList<Person::Ptr> m_persons;
    QString m_newPersonNames;

    QAction *m_actionAdd;
    QAction *m_actionEdit;
    QAction *m_actionRemove;

private slots:
    void addByAction();
    void editByAction();
    void removeByAction();

signals:
    void itemModelChanged(QAbstractItemModel *model);
    void newPersonNamesChanged(const QString &newPersonNames);
};
