#include "objectdatamodeldecorator.h"

//-----------------------------------------------------------------------------
ObjectDataModelDecorator::ObjectDataModelDecorator(ObjectDataModelDecorator::Handler handler, QObject *parent)
    : QIdentityProxyModel(parent)
    , m_handler(handler)
{
}

//-----------------------------------------------------------------------------
QVariant ObjectDataModelDecorator::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole)
        return QIdentityProxyModel::data(index, role);
    return index.isValid() ? m_handler(mapToSource(index), role) : QVariant();
}

//-----------------------------------------------------------------------------
ObjectPropertyColumnModelDecorator::ObjectPropertyColumnModelDecorator(int column, const QString &propertyName, Handler valueAdapter, QObject *parent)
    : m_handler(valueAdapter)
    , m_column(column)
{
}

//-----------------------------------------------------------------------------
QVariant ObjectPropertyColumnModelDecorator::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole || m_column != index.column())
        return QIdentityProxyModel::data(index, role);
    return index.isValid() ? m_handler(mapToSource(index)) : QVariant();
}
/*
//-----------------------------------------------------------------------------
int ObjectPropertyColumnModelDecorator::columnCount(const QModelIndex &index) const
{
   m_column = QIdentityProxyModel::columnCount(index);
   return m_column + 1;
}
*/