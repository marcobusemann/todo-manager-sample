#pragma once

#include <QAbstractProxyModel>

class ProxyModelUtils
{
public:
	static QModelIndex mapFromRoot(QAbstractProxyModel *model, const QModelIndex &rootIndex);
	static QModelIndex mapToRoot(QAbstractProxyModel *model, const QModelIndex &proxyIndex);
};