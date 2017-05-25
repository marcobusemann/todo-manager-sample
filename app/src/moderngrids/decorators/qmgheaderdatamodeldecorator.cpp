#include "qmgheaderdatamodeldecorator.h"

QmgHeaderDataModelDecorator::QmgHeaderDataModelDecorator(Qt::Orientation orientation, Handler handler, QObject *parent)
    : QIdentityProxyModel(parent)
    , m_orientation(orientation)
    , m_handler(handler)
{
}

QmgHeaderDataModelDecorator::QmgHeaderDataModelDecorator(Qt::Orientation orientation, RawHandler handler, QObject *parent)
    : QIdentityProxyModel(parent)
    , m_orientation(orientation)
    , m_rawHandler(handler)
{
}

QVariant QmgHeaderDataModelDecorator::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != m_orientation) return __super::headerData(section, orientation, role);
    return
        m_handler != nullptr ? m_handler(section, role) :
        m_rawHandler(section, role, [this, section, orientation, role]() -> QVariant { return QIdentityProxyModel::headerData(section, orientation, role); });
}

QmgSectionBasedHeaderDataModelDecorator::QmgSectionBasedHeaderDataModelDecorator(int section, Qt::Orientation orientation, Handler handler, QObject *parent)
    : QmgHeaderDataModelDecorator(orientation, handler, parent)
    , m_section(section)
{
}

QmgSectionBasedHeaderDataModelDecorator::QmgSectionBasedHeaderDataModelDecorator(int section, Qt::Orientation orientation, RawHandler handler, QObject *parent)
    : QmgHeaderDataModelDecorator(orientation, handler, parent)
    , m_section(section)
{
}

QVariant QmgSectionBasedHeaderDataModelDecorator::headerData(int section, Qt::Orientation orientation, int role) const
{
    return m_section == section ?
        __super::headerData(section, orientation, role) :
        QIdentityProxyModel::headerData(section, orientation, role);
}

QmgRoleBasedHeaderDataModelDecorator::QmgRoleBasedHeaderDataModelDecorator(int role, Qt::Orientation orientation, Handler handler, QObject *parent)
    : QmgHeaderDataModelDecorator(orientation, handler, parent)
    , m_role(role)
{
}

QmgRoleBasedHeaderDataModelDecorator::QmgRoleBasedHeaderDataModelDecorator(int role, Qt::Orientation orientation, RawHandler handler, QObject *parent)
    : QmgHeaderDataModelDecorator(orientation, handler, parent)
    , m_role(role)
{
}

QVariant QmgRoleBasedHeaderDataModelDecorator::headerData(int section, Qt::Orientation orientation, int role) const
{
    return m_role == role ?
        __super::headerData(section, orientation, role) :
        QIdentityProxyModel::headerData(section, orientation, role);
}

QmgSectionAndRoleBasedHeaderDataModelDecorator::QmgSectionAndRoleBasedHeaderDataModelDecorator(int section, int role, Qt::Orientation orientation, Handler handler, QObject *parent)
    : QmgHeaderDataModelDecorator(orientation, handler, parent)
    , m_role(role)
    , m_section(section)
{
}

QmgSectionAndRoleBasedHeaderDataModelDecorator::QmgSectionAndRoleBasedHeaderDataModelDecorator(int section, int role, Qt::Orientation orientation, RawHandler handler, QObject *parent)
    : QmgHeaderDataModelDecorator(orientation, handler, parent)
    , m_role(role)
    , m_section(section)
{
}

QVariant QmgSectionAndRoleBasedHeaderDataModelDecorator::headerData(int section, Qt::Orientation orientation, int role) const
{
    return m_role == role && m_section == section ?
        __super::headerData(section, orientation, role) :
        QIdentityProxyModel::headerData(section, orientation, role);
}