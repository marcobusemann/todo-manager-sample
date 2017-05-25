#include "qmgcommonmodelbuilder.h"

#include "../decorators/qmgalternaterowshadingmodeldecorator.h"

QmgCommonModelBuilder::QmgCommonModelBuilder(QAbstractItemModel *base, QObject *parent)
	: m_base(base)
	, m_parent(parent)
{
}

QmgCommonModelBuilder &QmgCommonModelBuilder::withData(QmgDataModelDecorator::Handler handler)
{
	return embeddNewModel(new QmgDataModelDecorator(handler, m_parent));
}

QmgCommonModelBuilder &QmgCommonModelBuilder::withData(QmgDataModelDecorator::RawHandler handler)
{
	return embeddNewModel(new QmgDataModelDecorator(handler, m_parent));
}

QmgCommonModelBuilder &QmgCommonModelBuilder::withData(int role, QmgDataModelDecorator::Handler handler)
{
	return embeddNewModel(new QmgRoleBasedDataModelDecorator(role, handler, m_parent));
}

QmgCommonModelBuilder &QmgCommonModelBuilder::withData(int role, QmgDataModelDecorator::RawHandler handler)
{
	return embeddNewModel(new QmgRoleBasedDataModelDecorator(role, handler, m_parent));
}

QmgDataModelBuilder QmgCommonModelBuilder::withData()
{
return QmgDataModelBuilder(m_base, m_parent);
}

QmgCommonModelBuilder &QmgCommonModelBuilder::withFetchMore(QmgFetchMoreModelDecorator::CanFetchMoreHandler canFetchMoreHandler, QmgFetchMoreModelDecorator::FetchMoreHandler fetchMoreHandler)
{
	return embeddNewModel(new QmgFetchMoreModelDecorator(canFetchMoreHandler, fetchMoreHandler, m_parent));
}

QmgCommonModelBuilder &QmgCommonModelBuilder::withHorizontalHeaderData(QmgHeaderDataModelDecorator::Handler handler)
{
	return embeddNewModel(new QmgHeaderDataModelDecorator(Qt::Horizontal, handler, m_parent));
}

QmgCommonModelBuilder &QmgCommonModelBuilder::withVerticalHeaderData(QmgHeaderDataModelDecorator::Handler handler)
{
	return embeddNewModel(new QmgHeaderDataModelDecorator(Qt::Horizontal, handler, m_parent));
}

QmgHeaderDataModelBuilder QmgCommonModelBuilder::withHeaderData()
{
	return QmgHeaderDataModelBuilder(m_base, m_parent);
}

QmgCommonModelBuilder &QmgCommonModelBuilder::withAlternateRowShading(const QColor &colorA, const QColor &colorB)
{
	return embeddNewModel(new QmgAlternateRowShadingModelDecorator(colorA, colorB, m_parent));
}

QmgCommonModelBuilder &QmgCommonModelBuilder::withSortAndFilter(std::function<void(QSortFilterProxyModel*)> handler)
{
	auto sortFilterModel = new QSortFilterProxyModel(m_parent);
	handler(sortFilterModel);
	return embeddNewModel(sortFilterModel);
}

QAbstractProxyModel *QmgCommonModelBuilder::build() 
{
	auto result = new QIdentityProxyModel(m_parent);
	result->setSourceModel(m_base);
	return result;
}

QmgCommonModelBuilder &QmgCommonModelBuilder::embeddNewModel(QAbstractProxyModel *model)
{
	model->setSourceModel(m_base);
	m_base = model;
	return *this;
}
