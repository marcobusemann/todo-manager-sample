#pragma once

#include "../qmgobservablevariantlistproxy.h"
#include "../builders/qmgcommonmodelbuilder.h"
#include "../builders/qmgcolumnmodelbuilder.h"

class QAbstractItemModel;

class QmgSourceModelBuilder final
{
public:
	QmgSourceModelBuilder(const QmgObservableVariantListProxy &list, QObject *parent);
	QmgCommonModelBuilder withColumns(int columns);
	QmgCommonModelBuilder withColumns(const QList<QmgColumnModelBuilder> columnModelBuilders);

private:
	QObject * const m_parent;
	QAbstractItemModel *m_base;
};
