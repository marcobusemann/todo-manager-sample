#include "observablevariantlistmodel.h"
#include "modellogger.h"

#include <QDebug>

//-----------------------------------------------------------------------------
ObservableVariantListModel::ObservableVariantListModel(QObject *parent)
    : QAbstractItemModel(parent)
{}

//-----------------------------------------------------------------------------
void ObservableVariantListModel::setSource(const QObservableVariantListProxy &source)
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
    auto result = parent.isValid() ? 0 : m_source.size();
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
        result = role == Qt::UserRole ? m_source.at(index.row()) : QVariant();
    ModelLogger::instance().logData("ObservableVariantListModel::data", index, role, result);
    return result;
}

//-----------------------------------------------------------------------------
QModelIndexList ObservableVariantListModel::match(const QModelIndex &start, int role, const QVariant &value, int hits, Qt::MatchFlags flags) const
{
    if (role != Qt::UserRole)
        return QAbstractItemModel::match(start, role, value, hits, flags);

    if (hits == -1)
        hits = m_source.size();

    QModelIndexList result;

    int items = m_source.size();
    int foundIndex = -1;
    do
    {
        foundIndex = m_source.indexOf(value, foundIndex == -1 ? start.row() : foundIndex);
        if (foundIndex > -1)
        {
            result.append(index(foundIndex, 0));
            hits--;
        }
    } while (foundIndex > -1 && hits > 0);

    return result;
}

//-----------------------------------------------------------------------------
void ObservableVariantListModel::linkTo(QObservableVariantListProxy &source)
{
    m_addBeforeScope = source.beforeAdd().attach([this](int index, const QVariant &) -> void
    {
        this->beginInsertRows(QModelIndex(), index, index);
    });

    m_addBatchBeforeScope = source.beforeAddBatch().attach([this](int index, const QVariant &, int amount) -> void
    {
        this->beginInsertRows(QModelIndex(), index, index + (amount - 1));
    });

    m_addAfterScope = source.afterAdd().attach([this](int, const QVariant &) -> void
    {
        this->endInsertRows();
    });

    m_addBatchAfterScope = source.afterAddBatch().attach([this](int, const QVariant &, int) -> void
    {
        this->endInsertRows();
    });

    m_removeBeforeScope = source.beforeRemove().attach([this](int index, const QVariant &) -> void
    {
        this->beginRemoveRows(QModelIndex(), index, index);
    });

    m_removeAfterScope = source.afterRemove().attach([this](int, const QVariant &) -> void
    {
        this->endRemoveRows();
    });

    m_updateAfterScope = source.afterUpdate().attach([this](int idx, const QVariant &) -> void
    {
        QModelIndex left = index(idx, 0);
        QModelIndex right = index(idx, columnCount() - 1);
        emit dataChanged(left, right);
    });

    m_clearBeforeScope = source.beforeClear().attach([this]()
    {
        this->beginResetModel();
    });

    m_clearAfterScope = source.afterClear().attach([this]()
    {
        this->endResetModel();
    });
}

//-----------------------------------------------------------------------------
void ObservableVariantListModel::unlinkFrom(QObservableVariantListProxy &source)
{
    m_addBeforeScope = QScopePtr();
    m_addAfterScope = QScopePtr();
    m_addBatchBeforeScope = QScopePtr();
    m_addBatchAfterScope = QScopePtr();
    m_removeBeforeScope = QScopePtr();
    m_removeAfterScope = QScopePtr();
    m_updateAfterScope = QScopePtr();
    m_clearBeforeScope = QScopePtr();
    m_clearAfterScope = QScopePtr();
}
