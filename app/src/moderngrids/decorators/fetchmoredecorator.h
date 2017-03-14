#pragma once

#include <qidentityproxymodel.h>

#include <functional>

class FetchMoreModelDecorator : public QIdentityProxyModel
{
public:
    typedef std::function<bool(const QModelIndex &)> CanFetchMoreHandler;
    typedef std::function<void(const QModelIndex &)> FetchMoreHandler;

public:
    FetchMoreModelDecorator(CanFetchMoreHandler canFetchMoreHandler, FetchMoreHandler fetchMoreHandler, QObject *parent = nullptr);
    virtual void fetchMore(const QModelIndex &parent) override;
    virtual bool canFetchMore(const QModelIndex &parent) const override;

private:
    CanFetchMoreHandler m_canFetchMoreHandler;
    FetchMoreHandler m_fetchMoreHandler;
};