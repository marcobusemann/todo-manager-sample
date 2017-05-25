#pragma once

#include <QIdentityProxyModel>

#include <functional>

/*

*/
class QmgHeaderDataModelDecorator : public QIdentityProxyModel
{
public:
    typedef std::function<QVariant(int, int)> Handler;
    typedef std::function<QVariant(int, int, std::function<QVariant()>)> RawHandler;

public:
	QmgHeaderDataModelDecorator(Qt::Orientation orientation, Handler handler, QObject *parent = nullptr);
	QmgHeaderDataModelDecorator(Qt::Orientation orientation, RawHandler handler, QObject *parent = nullptr);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    Qt::Orientation m_orientation;
    Handler m_handler;
    RawHandler m_rawHandler;
};

/*

*/
class QmgSectionBasedHeaderDataModelDecorator : public QmgHeaderDataModelDecorator
{
public:
	QmgSectionBasedHeaderDataModelDecorator(int section, Qt::Orientation orientation, Handler handler, QObject *parent = nullptr);
	QmgSectionBasedHeaderDataModelDecorator(int section, Qt::Orientation orientation, RawHandler handler, QObject *parent = nullptr);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    int m_section;
};

/*

*/
class QmgRoleBasedHeaderDataModelDecorator : public QmgHeaderDataModelDecorator
{
public:
	QmgRoleBasedHeaderDataModelDecorator(int role, Qt::Orientation orientation, Handler handler, QObject *parent = nullptr);
	QmgRoleBasedHeaderDataModelDecorator(int role, Qt::Orientation orientation, RawHandler handler, QObject *parent = nullptr);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    int m_role;
};

/*

*/
class QmgSectionAndRoleBasedHeaderDataModelDecorator : public QmgHeaderDataModelDecorator
{
public:
	QmgSectionAndRoleBasedHeaderDataModelDecorator(int section, int role, Qt::Orientation orientation, Handler handler, QObject *parent = nullptr);
	QmgSectionAndRoleBasedHeaderDataModelDecorator(int section, int role, Qt::Orientation orientation, RawHandler handler, QObject *parent = nullptr);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    int m_role;
    int m_section;
};
