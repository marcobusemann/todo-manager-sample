#pragma once

#include <QIdentityProxyModel>

#include <functional>

/*

*/
class HeaderDataModelDecorator : public QIdentityProxyModel
{
public:
    typedef std::function<QVariant(int, int)> Handler;
    typedef std::function<QVariant(int, int, std::function<QVariant()>)> RawHandler;

public:
    HeaderDataModelDecorator(Qt::Orientation orientation, Handler handler, QObject *parent = nullptr);
    HeaderDataModelDecorator(Qt::Orientation orientation, RawHandler handler, QObject *parent = nullptr);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    Qt::Orientation m_orientation;
    Handler m_handler;
    RawHandler m_rawHandler;
};

/*

*/
class SectionBasedHeaderDataModelDecorator : public HeaderDataModelDecorator
{
public:
    SectionBasedHeaderDataModelDecorator(int section, Qt::Orientation orientation, Handler handler, QObject *parent = nullptr);
    SectionBasedHeaderDataModelDecorator(int section, Qt::Orientation orientation, RawHandler handler, QObject *parent = nullptr);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    int m_section;
};

/*

*/
class RoleBasedHeaderDataModelDecorator : public HeaderDataModelDecorator
{
public:
    RoleBasedHeaderDataModelDecorator(int role, Qt::Orientation orientation, Handler handler, QObject *parent = nullptr);
    RoleBasedHeaderDataModelDecorator(int role, Qt::Orientation orientation, RawHandler handler, QObject *parent = nullptr);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    int m_role;
};

/*

*/
class SectionAndRoleBasedHeaderDataModelDecorator : public HeaderDataModelDecorator
{
public:
    SectionAndRoleBasedHeaderDataModelDecorator(int section, int role, Qt::Orientation orientation, Handler handler, QObject *parent = nullptr);
    SectionAndRoleBasedHeaderDataModelDecorator(int section, int role, Qt::Orientation orientation, RawHandler handler, QObject *parent = nullptr);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    int m_role;
    int m_section;
};
