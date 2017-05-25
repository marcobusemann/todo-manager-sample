#include "qmgcolumnmodeldecorator.h"
#include "../qmgmodellogger.h"

QmgColumnModelDecorator::QmgColumnModelDecorator(int columns, QObject *parent)
    : QIdentityProxyModel(parent)
    , m_columns(columns)
{}

int QmgColumnModelDecorator::columnCount(const QModelIndex &index) const
{
    QmgModelLogger::instance().logColumnCount("ColumnModelDecorator::columnCount", index, m_columns);
    return m_columns;
}

bool QmgColumnModelDecorator::hasChildren(const QModelIndex &parent) const
{
    return (rowCount(parent) > 0) && (columnCount(parent) > 0);
}
