#include "qmgsourcemodelbuilder.h"

#include "../decorators/qmgcolumnmodeldecorator.h"
#include "../qmgobservablevariantlistmodel.h"

QmgSourceModelBuilder::QmgSourceModelBuilder(const QmgObservableVariantListProxy &list, QObject *parent)
	: m_parent(parent)
{
	auto model = new QmgObservableVariantListModel(parent);
	model->setSource(list);
	m_base = model;
}

QmgCommonModelBuilder QmgSourceModelBuilder::withColumns(int columns)
{
	auto model = new QmgColumnModelDecorator(columns, m_parent);
	model->setSourceModel(m_base);
	return QmgCommonModelBuilder(model, m_parent);
}

QmgCommonModelBuilder QmgSourceModelBuilder::withColumns(const QList<QmgColumnModelBuilder> columnModelBuilders)
{
	auto model = new QmgColumnModelDecorator(columnModelBuilders.size(), m_parent);
	model->setSourceModel(m_base);

	QAbstractItemModel *last = model;
	for (int i = 0; i < columnModelBuilders.size(); i++) {
		const auto &columnBuilder = columnModelBuilders[i];
		last = columnBuilder.build(i, last);
	}

	return QmgCommonModelBuilder(last, m_parent);
}
