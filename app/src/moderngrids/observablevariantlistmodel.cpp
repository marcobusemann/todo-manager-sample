#include "observablevariantlistmodel.h"
#include "modellogger.h"

#include <QDebug>

//-----------------------------------------------------------------------------
ObservableVariantListModel::ObservableVariantListModel(QObject *parent)
    : QAbstractItemModel(parent)
{}

//-----------------------------------------------------------------------------
void ObservableVariantListModel::setSource(const std::shared_ptr<IObservableVariantList> &source)
{
    unlinkFrom(m_source);
    m_source = source;
    linkTo(m_source);
}

//-----------------------------------------------------------------------------
QModelIndex ObservableVariantListModel::index(int row, int column, const QModelIndex &parent) const
{
    auto result = parent.isValid() ? QModelIndex() : createIndex(row, column);
    ModelLogger::instance().logIndex("ObservableVariantListModel::index", row, column, parent, result);
    return result;
}

//-----------------------------------------------------------------------------
QModelIndex ObservableVariantListModel::parent(const QModelIndex &index) const
{
    ModelLogger::instance().logParent("ObservableVariantListModel::parent", index, QModelIndex());
    return QModelIndex();
}

//-----------------------------------------------------------------------------
int ObservableVariantListModel::rowCount(const QModelIndex &parent) const
{
    auto result = parent.isValid() ? 0 : m_source->size();
    ModelLogger::instance().logRowCount("ObservableVariantListModel::rowCount", parent, result);
    return result;
}

//-----------------------------------------------------------------------------
int ObservableVariantListModel::columnCount(const QModelIndex &index) const
{
    ModelLogger::instance().logRowCount("ObservableVariantListModel::columnCount", index, 0);
    return 0;
}

//-----------------------------------------------------------------------------
QVariant ObservableVariantListModel::data(const QModelIndex &index, int role) const
{
    QVariant result;
    if (index.isValid())
        result = role == Qt::UserRole ? m_source->at(index.row()) : QVariant();
    ModelLogger::instance().logData("ObservableVariantListModel::data", index, role, result);
    return result;
}

//-----------------------------------------------------------------------------
QModelIndexList ObservableVariantListModel::match(const QModelIndex &start, int role, const QVariant &value, int hits, Qt::MatchFlags flags) const
{
    if (role != Qt::UserRole)
        return QAbstractItemModel::match(start, role, value, hits, flags);

    if (hits == -1)
        hits = m_source->size();

    QModelIndexList result;

    int items = m_source->size();
    int foundIndex = -1;
    do
    {
        foundIndex = m_source->indexOf(value, foundIndex == -1 ? start.row() : foundIndex);
        if (foundIndex > -1)
        {
            result.append(index(foundIndex, 0));
            hits--;
        }
    } while (foundIndex > -1 && hits > 0);

    return result;
}

//-----------------------------------------------------------------------------
void ObservableVariantListModel::linkTo(IObservableVariantList::Ptr list)
{
    list->setOnBeforeAdd([this](int index, const QVariant &) -> void
    {
        this->beginInsertRows(QModelIndex(), index, index);
    });
    list->setOnBeforeAddBatch([this](int index, const QVariant &, int amount) -> void
    {
        this->beginInsertRows(QModelIndex(), index, index + (amount - 1));
    });
    list->setOnAdd([this](int, const QVariant &) -> void
    {
        this->endInsertRows();
    });
    list->setOnAddBatch([this](int, const QVariant &, int) -> void
    {
        this->endInsertRows();
    });

    list->setOnBeforeRemove([this](int index, const QVariant &) -> void
    {
        this->beginRemoveRows(QModelIndex(), index, index);
    });
    list->setOnRemove([this](int, const QVariant &) -> void
    {
        this->endRemoveRows();
    });
    list->setOnBeforeClear([this]()
    {
        this->beginResetModel();
    });
    list->setOnClear([this]() 
    {
        this->endResetModel();
    });
}

//-----------------------------------------------------------------------------
void ObservableVariantListModel::unlinkFrom(IObservableVariantList::Ptr list)
{
    if (!list) return;
    list->setOnBeforeAdd(nullptr);
    list->setOnBeforeRemove(nullptr);
    list->setOnBeforeClear(nullptr);
    list->setOnAdd(nullptr);
    list->setOnRemove(nullptr);
    list->setOnClear(nullptr);
}
