#include "columnmodeldecorator.h"
#include "../modellogger.h"

//-----------------------------------------------------------------------------
ColumnModelDecorator::ColumnModelDecorator(int columns, QObject *parent)
    : QIdentityProxyModel(parent)
    , m_columns(columns)
{}

//-----------------------------------------------------------------------------
int ColumnModelDecorator::columnCount(const QModelIndex &index) const
{
    ModelLogger::instance().logColumnCount("ColumnModelDecorator::columnCount", index, m_columns);
    return m_columns;
}

//-----------------------------------------------------------------------------
bool ColumnModelDecorator::hasChildren(const QModelIndex &parent) const
{
    return (rowCount(parent) > 0) && (columnCount(parent) > 0);
}
