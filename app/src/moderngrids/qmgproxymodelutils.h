#pragma once

#include <QAbstractProxyModel>

class QmgProxyModelUtils
{
public:
	static QModelIndex mapFromRoot(QAbstractProxyModel *model, const QModelIndex &rootIndex);
	static QModelIndex mapToRoot(QAbstractProxyModel *model, const QModelIndex &proxyIndex);
};