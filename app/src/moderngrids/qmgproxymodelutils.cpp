#include "qmgproxymodelutils.h"

QModelIndex QmgProxyModelUtils::mapFromRoot(QAbstractProxyModel *model, const QModelIndex &rootIndex)
{
	const QAbstractProxyModel *origin = model;
	QModelIndex result = model->mapFromSource(rootIndex);

	while ((origin = dynamic_cast<QAbstractProxyModel*>(origin->sourceModel())) != nullptr)
		result = origin->mapFromSource(result);

	return result;
}

QModelIndex QmgProxyModelUtils::mapToRoot(QAbstractProxyModel *model, const QModelIndex &proxyIndex)
{
	const QAbstractProxyModel *origin = model;
	QModelIndex result = model->mapToSource(proxyIndex);

	while ((origin = dynamic_cast<QAbstractProxyModel*>(origin->sourceModel())) != nullptr)
		result = origin->mapToSource(result);

	return result;
}