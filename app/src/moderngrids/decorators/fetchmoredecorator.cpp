#include "fetchmoredecorator.h"

FetchMoreModelDecorator::FetchMoreModelDecorator(CanFetchMoreHandler canFetchMoreHandler, FetchMoreHandler fetchMoreHandler, QObject * parent)
    : QIdentityProxyModel(parent)
    , m_canFetchMoreHandler(canFetchMoreHandler)
    , m_fetchMoreHandler(fetchMoreHandler)
{
}

void FetchMoreModelDecorator::fetchMore(const QModelIndex &parent)
{
    m_fetchMoreHandler(parent);
}

bool FetchMoreModelDecorator::canFetchMore(const QModelIndex &parent) const
{
    return m_canFetchMoreHandler(parent);
}
