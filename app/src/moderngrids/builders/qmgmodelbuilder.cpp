#include "qmgmodelbuilder.h"

QmgModelBuilder::QmgModelBuilder(QObject *parent)
	: m_parent(parent)
{}

QmgSourceModelBuilder QmgModelBuilder::AModelFor(const QmgObservableVariantListProxy &list, QObject *parent)
{
	return QmgSourceModelBuilder(list, parent);
}

QmgCommonModelBuilder QmgModelBuilder::AListModelFor(const QmgObservableVariantListProxy &list, QObject *parent)
{
	return QmgSourceModelBuilder(list, parent)
		.withColumns(1);
}

QmgModelBuilder QmgModelBuilder::AModel(QObject *parent)
{
	return QmgModelBuilder(parent);
}

QmgSourceModelBuilder QmgModelBuilder::of(const QmgObservableVariantListProxy &list)
{
	return QmgSourceModelBuilder(list, m_parent);
}
