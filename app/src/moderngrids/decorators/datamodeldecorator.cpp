#include "datamodeldecorator.h"
#include <QSet>

//-----------------------------------------------------------------------------
DataModelDecorator::DataModelDecorator(Handler handler, QObject *parent)
    : QIdentityProxyModel(parent)
    , m_handler(handler)
    , m_excludeRoles(QSet<int>() << Qt::UserRole)
{
}

//-----------------------------------------------------------------------------
DataModelDecorator::DataModelDecorator(RawHandler handler, QObject *parent)
    : QIdentityProxyModel(parent)
    , m_rawHandler(handler)
    , m_excludeRoles(QSet<int>() << Qt::UserRole)
{
}

//-----------------------------------------------------------------------------
DataModelDecorator::DataModelDecorator(RawRoleHandler handler, QObject *parent)
    : QIdentityProxyModel(parent)
    , m_rawRoleHandler(handler)
    , m_excludeRoles(QSet<int>() << Qt::UserRole)
{
}

//-----------------------------------------------------------------------------
DataModelDecorator::DataModelDecorator(const QSet<int> &roleExcludes, Handler handler, QObject *parent)
    : QIdentityProxyModel(parent)
    , m_handler(handler)
    , m_excludeRoles(roleExcludes)
{
}

//-----------------------------------------------------------------------------
DataModelDecorator::DataModelDecorator(const QSet<int> &roleExcludes, RawHandler handler, QObject *parent)
    : QIdentityProxyModel(parent)
    , m_rawHandler(handler)
    , m_excludeRoles(roleExcludes)
{
}

//-----------------------------------------------------------------------------
DataModelDecorator::DataModelDecorator(const QSet<int> &roleExcludes, RawRoleHandler handler, QObject *parent)
    : QIdentityProxyModel(parent)
    , m_rawRoleHandler(handler)
    , m_excludeRoles(roleExcludes)
{
}

//-----------------------------------------------------------------------------
QVariant DataModelDecorator::data(const QModelIndex &index, int role) const
{
    if (m_excludeRoles.contains(role))
        return QIdentityProxyModel::data(index, role);
    if (!index.isValid())
        return QVariant();
    return
        m_handler != nullptr ? m_handler(mapToSource(index), role) :
        m_rawHandler != nullptr ? m_rawHandler(mapToSource(index), role, [this, index, role]() -> QVariant { return QIdentityProxyModel::data(index, role); }) :
        m_rawRoleHandler(mapToSource(index), role, [this, index](int role) -> QVariant { return QIdentityProxyModel::data(index, role); });
}

//-----------------------------------------------------------------------------
ColumnBasedDataModelDecorator::ColumnBasedDataModelDecorator(int column, Handler handler, QObject *parent)
    : DataModelDecorator(handler, parent)
    , m_column(column)
{
}

//-----------------------------------------------------------------------------
ColumnBasedDataModelDecorator::ColumnBasedDataModelDecorator(int column, const QSet<int> &roleExcludes, Handler handler, QObject *parent)
    : DataModelDecorator(roleExcludes, handler, parent)
    , m_column(column)
{
}

//-----------------------------------------------------------------------------
ColumnBasedDataModelDecorator::ColumnBasedDataModelDecorator(int column, RawHandler handler, QObject *parent)
    : DataModelDecorator(handler, parent)
    , m_column(column)
{
}

//-----------------------------------------------------------------------------
ColumnBasedDataModelDecorator::ColumnBasedDataModelDecorator(int column, const QSet<int> &roleExcludes, RawHandler handler, QObject *parent)
    : DataModelDecorator(roleExcludes, handler, parent)
    , m_column(column)
{
}

//-----------------------------------------------------------------------------
ColumnBasedDataModelDecorator::ColumnBasedDataModelDecorator(int column, RawRoleHandler handler, QObject *parent)
    : DataModelDecorator(handler, parent)
    , m_column(column)
{
}

//-----------------------------------------------------------------------------
ColumnBasedDataModelDecorator::ColumnBasedDataModelDecorator(int column, const QSet<int> &roleExcludes, RawRoleHandler handler, QObject *parent)
    : DataModelDecorator(roleExcludes, handler, parent)
    , m_column(column)
{
}

//-----------------------------------------------------------------------------
QVariant ColumnBasedDataModelDecorator::data(const QModelIndex &index, int role) const
{
    return index.column() == m_column ?
        __super::data(index, role) :
        QIdentityProxyModel::data(index, role);
}

//-----------------------------------------------------------------------------
RoleBasedDataModelDecorator::RoleBasedDataModelDecorator(int role, Handler handler, QObject *parent)
    : DataModelDecorator(handler, parent)
    , m_role(role)
{
}

//-----------------------------------------------------------------------------
RoleBasedDataModelDecorator::RoleBasedDataModelDecorator(int role, RawHandler handler, QObject *parent)
    : DataModelDecorator(handler, parent)
    , m_role(role)
{
}

//-----------------------------------------------------------------------------
RoleBasedDataModelDecorator::RoleBasedDataModelDecorator(int role, RawRoleHandler handler, QObject *parent)
    : DataModelDecorator(handler, parent)
    , m_role(role)
{
}

//-----------------------------------------------------------------------------
QVariant RoleBasedDataModelDecorator::data(const QModelIndex &index, int role) const
{
    return role == m_role ?
        __super::data(index, role) :
        QIdentityProxyModel::data(index, role);
}

//-----------------------------------------------------------------------------
ColumnAndRoleBasedDataModelDecorator::ColumnAndRoleBasedDataModelDecorator(int column, int role, Handler handler, QObject *parent)
    : DataModelDecorator(handler, parent)
    , m_column(column)
    , m_role(role)
{
}

//-----------------------------------------------------------------------------
ColumnAndRoleBasedDataModelDecorator::ColumnAndRoleBasedDataModelDecorator(int column, int role, RawHandler handler, QObject *parent)
    : DataModelDecorator(handler, parent)
    , m_column(column)
    , m_role(role)
{
}

//-----------------------------------------------------------------------------
ColumnAndRoleBasedDataModelDecorator::ColumnAndRoleBasedDataModelDecorator(int column, int role, RawRoleHandler handler, QObject *parent)
    : DataModelDecorator(handler, parent)
    , m_column(column)
    , m_role(role)
{
}

//-----------------------------------------------------------------------------
QVariant ColumnAndRoleBasedDataModelDecorator::data(const QModelIndex &index, int role) const
{
    return index.column() == m_column && role == m_role ?
        __super::data(index, role) :
        QIdentityProxyModel::data(index, role);
}