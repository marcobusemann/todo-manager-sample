#include "qmgobjectdatamodeldecorator.h"

QmgObjectDataModelDecorator::QmgObjectDataModelDecorator(QmgObjectDataModelDecorator::Handler handler, QObject *parent)
    : QIdentityProxyModel(parent)
    , m_handler(handler)
{
}

QVariant QmgObjectDataModelDecorator::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole)
        return QIdentityProxyModel::data(index, role);
    return index.isValid() ? m_handler(mapToSource(index), role) : QVariant();
}

QmgObjectPropertyColumnModelDecorator::QmgObjectPropertyColumnModelDecorator(int column, const QString &propertyName, Handler valueAdapter, QObject *parent)
    : m_handler(valueAdapter)
    , m_column(column)
{
}

QVariant QmgObjectPropertyColumnModelDecorator::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole || m_column != index.column())
        return QIdentityProxyModel::data(index, role);
    return index.isValid() ? m_handler(mapToSource(index)) : QVariant();
}

/*
int QmgObjectPropertyColumnModelDecorator::columnCount(const QModelIndex &index) const
{
   m_column = QIdentityProxyModel::columnCount(index);
   return m_column + 1;
}
*/