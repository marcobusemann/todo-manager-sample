#pragma once

#include <QIdentityProxyModel>
#include <QSet>

#include <functional>

class DataModelDecorator : public QIdentityProxyModel
{
public:
    typedef std::function<QVariant(const QModelIndex &, int)> Handler;
    typedef std::function<QVariant(const QModelIndex &, int, std::function<QVariant()>)> RawHandler;
    typedef std::function<QVariant(const QModelIndex &, int, std::function<QVariant(int)>)> RawRoleHandler;

public:
    DataModelDecorator(Handler handler, QObject *parent = nullptr);
    DataModelDecorator(RawHandler handler, QObject *parent = nullptr);
    DataModelDecorator(RawRoleHandler handler, QObject *parent = nullptr);
    DataModelDecorator(const QSet<int> &roleExcludes, Handler handler, QObject *parent = nullptr);
    DataModelDecorator(const QSet<int> &roleExcludes, RawHandler handler, QObject *parent = nullptr);
    DataModelDecorator(const QSet<int> &roleExcludes, RawRoleHandler handler, QObject *parent = nullptr);
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    Handler m_handler;
    RawHandler m_rawHandler;
    RawRoleHandler m_rawRoleHandler;
    QSet<int> m_excludeRoles;
};

class ColumnBasedDataModelDecorator : public DataModelDecorator
{
public:
    ColumnBasedDataModelDecorator(int column, Handler handler, QObject *parent = nullptr);
    ColumnBasedDataModelDecorator(int column, const QSet<int> &roleExcludes, Handler handler, QObject *parent = nullptr);
    ColumnBasedDataModelDecorator(int column, RawHandler handler, QObject *parent = nullptr);
    ColumnBasedDataModelDecorator(int column, const QSet<int> &roleExcludes, RawHandler handler, QObject *parent = nullptr);
    ColumnBasedDataModelDecorator(int column, RawRoleHandler handler, QObject *parent = nullptr);
    ColumnBasedDataModelDecorator(int column, const QSet<int> &roleExcludes, RawRoleHandler handler, QObject *parent = nullptr);
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    int m_column;
};

class RoleBasedDataModelDecorator : public DataModelDecorator
{
public:
    RoleBasedDataModelDecorator(int role, Handler handler, QObject *parent = nullptr);
    RoleBasedDataModelDecorator(int role, RawHandler handler, QObject *parent = nullptr);
    RoleBasedDataModelDecorator(int role, RawRoleHandler handler, QObject *parent = nullptr);
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    int m_role;
};

class ColumnAndRoleBasedDataModelDecorator : public DataModelDecorator
{
public:
    ColumnAndRoleBasedDataModelDecorator(int column, int role, Handler handler, QObject *parent = nullptr);
    ColumnAndRoleBasedDataModelDecorator(int column, int role, RawHandler handler, QObject *parent = nullptr);
    ColumnAndRoleBasedDataModelDecorator(int column, int role, RawRoleHandler handler, QObject *parent = nullptr);
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    int m_column;
    int m_role;
};
