#pragma once

#include <QAbstractListModel>
#include <QList>

#include <data/person.h>

class PersonListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    PersonListModel(QObject *parent = nullptr);
    void setItems(const QList<Person::Ptr> &items);
    const QList<Person::Ptr> &getItems() const;

    void addItem(const Person::Ptr &item);
    void removeItem(const QList<QPersistentModelIndex> &item);

    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual int rowCount(const QModelIndex &parent) const override;

private:
    QList<Person::Ptr> m_items;
};
