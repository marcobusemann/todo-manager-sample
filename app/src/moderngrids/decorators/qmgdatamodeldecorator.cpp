#include "qmgdatamodeldecorator.h"
#include <QSet>

QmgDataModelDecorator::QmgDataModelDecorator(Handler handler, QObject *parent)
    : QIdentityProxyModel(parent)
    , m_handler(handler)
    , m_excludeRoles(QSet<int>() << Qt::UserRole)
{
}

QmgDataModelDecorator::QmgDataModelDecorator(RawHandler handler, QObject *parent)
    : QIdentityProxyModel(parent)
    , m_rawHandler(handler)
    , m_excludeRoles(QSet<int>() << Qt::UserRole)
{
}

QmgDataModelDecorator::QmgDataModelDecorator(RawRoleHandler handler, QObject *parent)
    : QIdentityProxyModel(parent)
    , m_rawRoleHandler(handler)
    , m_excludeRoles(QSet<int>() << Qt::UserRole)
{
}

QmgDataModelDecorator::QmgDataModelDecorator(const QSet<int> &roleExcludes, Handler handler, QObject *parent)
    : QIdentityProxyModel(parent)
    , m_handler(handler)
    , m_excludeRoles(roleExcludes)
{
}

QmgDataModelDecorator::QmgDataModelDecorator(const QSet<int> &roleExcludes, RawHandler handler, QObject *parent)
    : QIdentityProxyModel(parent)
    , m_rawHandler(handler)
    , m_excludeRoles(roleExcludes)
{
}

QmgDataModelDecorator::QmgDataModelDecorator(const QSet<int> &roleExcludes, RawRoleHandler handler, QObject *parent)
    : QIdentityProxyModel(parent)
    , m_rawRoleHandler(handler)
    , m_excludeRoles(roleExcludes)
{
}

QVariant QmgDataModelDecorator::data(const QModelIndex &index, int role) const
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

QmgColumnBasedDataModelDecorator::QmgColumnBasedDataModelDecorator(int column, Handler handler, QObject *parent)
    : QmgDataModelDecorator(handler, parent)
    , m_column(column)
{
}

QmgColumnBasedDataModelDecorator::QmgColumnBasedDataModelDecorator(int column, const QSet<int> &roleExcludes, Handler handler, QObject *parent)
    : QmgDataModelDecorator(roleExcludes, handler, parent)
    , m_column(column)
{
}

QmgColumnBasedDataModelDecorator::QmgColumnBasedDataModelDecorator(int column, RawHandler handler, QObject *parent)
    : QmgDataModelDecorator(handler, parent)
    , m_column(column)
{
}

QmgColumnBasedDataModelDecorator::QmgColumnBasedDataModelDecorator(int column, const QSet<int> &roleExcludes, RawHandler handler, QObject *parent)
    : QmgDataModelDecorator(roleExcludes, handler, parent)
    , m_column(column)
{
}

QmgColumnBasedDataModelDecorator::QmgColumnBasedDataModelDecorator(int column, RawRoleHandler handler, QObject *parent)
    : QmgDataModelDecorator(handler, parent)
    , m_column(column)
{
}

QmgColumnBasedDataModelDecorator::QmgColumnBasedDataModelDecorator(int column, const QSet<int> &roleExcludes, RawRoleHandler handler, QObject *parent)
    : QmgDataModelDecorator(roleExcludes, handler, parent)
    , m_column(column)
{
}

QVariant QmgColumnBasedDataModelDecorator::data(const QModelIndex &index, int role) const
{
    return index.column() == m_column ?
        __super::data(index, role) :
        QIdentityProxyModel::data(index, role);
}

QmgRoleBasedDataModelDecorator::QmgRoleBasedDataModelDecorator(int role, Handler handler, QObject *parent)
    : QmgDataModelDecorator(handler, parent)
    , m_role(role)
{
}

QmgRoleBasedDataModelDecorator::QmgRoleBasedDataModelDecorator(int role, RawHandler handler, QObject *parent)
    : QmgDataModelDecorator(handler, parent)
    , m_role(role)
{
}

QmgRoleBasedDataModelDecorator::QmgRoleBasedDataModelDecorator(int role, RawRoleHandler handler, QObject *parent)
    : QmgDataModelDecorator(handler, parent)
    , m_role(role)
{
}

QVariant QmgRoleBasedDataModelDecorator::data(const QModelIndex &index, int role) const
{
    return role == m_role ?
        __super::data(index, role) :
        QIdentityProxyModel::data(index, role);
}

QmgColumnAndRoleBasedDataModelDecorator::QmgColumnAndRoleBasedDataModelDecorator(int column, int role, Handler handler, QObject *parent)
    : QmgDataModelDecorator(handler, parent)
    , m_column(column)
    , m_role(role)
{
}

QmgColumnAndRoleBasedDataModelDecorator::QmgColumnAndRoleBasedDataModelDecorator(int column, int role, RawHandler handler, QObject *parent)
    : QmgDataModelDecorator(handler, parent)
    , m_column(column)
    , m_role(role)
{
}

QmgColumnAndRoleBasedDataModelDecorator::QmgColumnAndRoleBasedDataModelDecorator(int column, int role, RawRoleHandler handler, QObject *parent)
    : QmgDataModelDecorator(handler, parent)
    , m_column(column)
    , m_role(role)
{
}

QVariant QmgColumnAndRoleBasedDataModelDecorator::data(const QModelIndex &index, int role) const
{
    return index.column() == m_column && role == m_role ?
        __super::data(index, role) :
        QIdentityProxyModel::data(index, role);
}