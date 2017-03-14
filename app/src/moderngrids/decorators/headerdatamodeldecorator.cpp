#include "headerdatamodeldecorator.h"

//-----------------------------------------------------------------------------
HeaderDataModelDecorator::HeaderDataModelDecorator(Qt::Orientation orientation, Handler handler, QObject *parent)
    : QIdentityProxyModel(parent)
    , m_orientation(orientation)
    , m_handler(handler)
{
}

//-----------------------------------------------------------------------------
HeaderDataModelDecorator::HeaderDataModelDecorator(Qt::Orientation orientation, RawHandler handler, QObject *parent)
    : QIdentityProxyModel(parent)
    , m_orientation(orientation)
    , m_rawHandler(handler)
{
}

//-----------------------------------------------------------------------------
QVariant HeaderDataModelDecorator::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != m_orientation) return __super::headerData(section, orientation, role);
    return
        m_handler != nullptr ? m_handler(section, role) :
        m_rawHandler(section, role, [this, section, orientation, role]() -> QVariant { return QIdentityProxyModel::headerData(section, orientation, role); });
}

//-----------------------------------------------------------------------------
SectionBasedHeaderDataModelDecorator::SectionBasedHeaderDataModelDecorator(int section, Qt::Orientation orientation, Handler handler, QObject *parent)
    : HeaderDataModelDecorator(orientation, handler, parent)
    , m_section(section)
{
}

//-----------------------------------------------------------------------------
SectionBasedHeaderDataModelDecorator::SectionBasedHeaderDataModelDecorator(int section, Qt::Orientation orientation, RawHandler handler, QObject *parent)
    : HeaderDataModelDecorator(orientation, handler, parent)
    , m_section(section)
{
}

//-----------------------------------------------------------------------------
QVariant SectionBasedHeaderDataModelDecorator::headerData(int section, Qt::Orientation orientation, int role) const
{
    return m_section == section ?
        __super::headerData(section, orientation, role) :
        QIdentityProxyModel::headerData(section, orientation, role);
}

//-----------------------------------------------------------------------------
RoleBasedHeaderDataModelDecorator::RoleBasedHeaderDataModelDecorator(int role, Qt::Orientation orientation, Handler handler, QObject *parent)
    : HeaderDataModelDecorator(orientation, handler, parent)
    , m_role(role)
{
}

//-----------------------------------------------------------------------------
RoleBasedHeaderDataModelDecorator::RoleBasedHeaderDataModelDecorator(int role, Qt::Orientation orientation, RawHandler handler, QObject *parent)
    : HeaderDataModelDecorator(orientation, handler, parent)
    , m_role(role)
{
}

//-----------------------------------------------------------------------------
QVariant RoleBasedHeaderDataModelDecorator::headerData(int section, Qt::Orientation orientation, int role) const
{
    return m_role == role ?
        __super::headerData(section, orientation, role) :
        QIdentityProxyModel::headerData(section, orientation, role);
}

//-----------------------------------------------------------------------------
SectionAndRoleBasedHeaderDataModelDecorator::SectionAndRoleBasedHeaderDataModelDecorator(int section, int role, Qt::Orientation orientation, Handler handler, QObject *parent)
    : HeaderDataModelDecorator(orientation, handler, parent)
    , m_role(role)
    , m_section(section)
{
}

//-----------------------------------------------------------------------------
SectionAndRoleBasedHeaderDataModelDecorator::SectionAndRoleBasedHeaderDataModelDecorator(int section, int role, Qt::Orientation orientation, RawHandler handler, QObject *parent)
    : HeaderDataModelDecorator(orientation, handler, parent)
    , m_role(role)
    , m_section(section)
{
}

//-----------------------------------------------------------------------------
QVariant SectionAndRoleBasedHeaderDataModelDecorator::headerData(int section, Qt::Orientation orientation, int role) const
{
    return m_role == role && m_section == section ?
        __super::headerData(section, orientation, role) :
        QIdentityProxyModel::headerData(section, orientation, role);
}