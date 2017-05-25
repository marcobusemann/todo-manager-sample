#include "qmgfetchmoredecorator.h"

QmgFetchMoreModelDecorator::QmgFetchMoreModelDecorator(CanFetchMoreHandler canFetchMoreHandler, FetchMoreHandler fetchMoreHandler, QObject * parent)
    : QIdentityProxyModel(parent)
    , m_canFetchMoreHandler(canFetchMoreHandler)
    , m_fetchMoreHandler(fetchMoreHandler)
{
}

void QmgFetchMoreModelDecorator::fetchMore(const QModelIndex &parent)
{
    m_fetchMoreHandler(parent);
}

bool QmgFetchMoreModelDecorator::canFetchMore(const QModelIndex &parent) const
{
    return m_canFetchMoreHandler(parent);
}
