#include "personlistmodel.h"

PersonListModel::PersonListModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

void PersonListModel::setItems(const QList<Person::Ptr> &items)
{
    beginResetModel();
    m_items = items;
    endResetModel();
}

const QList<Person::Ptr> &PersonListModel::getItems() const
{
    return m_items;
}

void PersonListModel::addItem(const Person::Ptr &item)
{
    int row = m_items.size();
    beginInsertRows(QModelIndex(), row, row);
    m_items.append(item);
    endInsertRows();
}

void PersonListModel::removeItem(const QList<QPersistentModelIndex> &items)
{
    for (auto item : items) {
        int row = item.row();
        beginRemoveRows(QModelIndex(), row, row);
        m_items.removeAt(row);
        endRemoveRows();
    }
}

QVariant PersonListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (role != Qt::DisplayRole) return QVariant();

    auto item = m_items[index.row()];
    return QString("%1, %2").arg(item->getName(), item->getSurname());
}

int PersonListModel::rowCount(const QModelIndex &) const
{
    return m_items.size();
}
