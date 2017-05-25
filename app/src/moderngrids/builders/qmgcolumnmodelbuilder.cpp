#include "qmgcolumnmodelbuilder.h"

#include "../decorators/qmgheaderdatamodeldecorator.h"
#include "../decorators/qmgdatamodeldecorator.h"

QmgColumnModelBuilder QmgColumnModelBuilder::ACustom(QObject *parent) 
{
	QmgColumnModelBuilder tmp;
	tmp.m_parent = parent;
	return tmp;
}

QmgColumnModelBuilder &QmgColumnModelBuilder::withHeader(const QString &headerText, QIcon icon, Qt::Orientation orientation) 
{
	auto parent = this->getParent();
	m_factories.append([headerText, orientation, icon, parent](int column) {
		auto func = [headerText, icon](int, int role, std::function<QVariant()> rawDataGetter) -> QVariant {
			return
				role == Qt::DisplayRole ? headerText :
				role == Qt::DecorationRole ? icon :
				rawDataGetter();
		};
		return new QmgSectionBasedHeaderDataModelDecorator(column, orientation, func, parent);
	});
	return *this;
}

QmgColumnModelBuilder &QmgColumnModelBuilder::withMapData(int role, std::function<QVariant(const QModelIndex &index, const QVariant &rawValue)> mapHandler) 
{
	auto parent = this->getParent();
	m_factories.append([parent, role, mapHandler](int column) {
		auto func = [mapHandler](const QModelIndex &index, int, std::function<QVariant()> rawFunc) -> QVariant {
			return mapHandler(index, rawFunc());
		};
		return new QmgColumnAndRoleBasedDataModelDecorator(column, role, func, parent);
	});
	return *this;
}

QmgColumnModelBuilder &QmgColumnModelBuilder::withData(int role, std::function<QVariant(const QModelIndex &index)> mapHandler)
{
	auto parent = this->getParent();
	m_factories.append([parent, role, mapHandler](int column) {
		auto func = [mapHandler](const QModelIndex &index, int) -> QVariant {
			return mapHandler(index);
		};
		return new QmgColumnAndRoleBasedDataModelDecorator(column, role, func, parent);
	});
	return *this;
}

QmgColumnModelBuilder &QmgColumnModelBuilder::withConstData(int role, std::function<QVariant()> mapHandler) 
{
	auto parent = this->getParent();
	m_factories.append([parent, role, mapHandler](int column) {
		auto func = [mapHandler](const QModelIndex &, int) -> QVariant {
			return mapHandler();
		};
		return new QmgColumnAndRoleBasedDataModelDecorator(column, role, func, parent);
	});
	return *this;
}

QmgColumnModelBuilder &QmgColumnModelBuilder::withConstData(int role, const QVariant &value) 
{
	auto parent = this->getParent();
	m_factories.append([parent, role, value](int column) {
		auto func = [value](const QModelIndex &, int) -> QVariant {
			return value;
		};
		return new QmgColumnAndRoleBasedDataModelDecorator(column, role, func, parent);
	});
	return *this;
}

QmgColumnModelBuilder &QmgColumnModelBuilder::withData(std::function<QVariant(const QModelIndex &index, int role, std::function<QVariant()> rawValueGetter)> mapHandler) 
{
	auto parent = this->getParent();
	m_factories.append([parent, mapHandler](int column) {
		auto func = [mapHandler](const QModelIndex &index, int role, std::function<QVariant()> rawValueGetter) -> QVariant {
			return mapHandler(index, role, rawValueGetter);
		};
		return new QmgColumnBasedDataModelDecorator(column, func, parent);
	});
	return *this;
}

QAbstractItemModel *QmgColumnModelBuilder::build(int column, QAbstractItemModel *startModel) const 
{
	QAbstractItemModel *last = startModel;
	for (auto factory : m_factories) {
		auto model = factory(column);
		if (last != nullptr)
			model->setSourceModel(last);
		last = model;
	}
	return last;
}

QObject *QmgColumnModelBuilder::getParent() const
{ 
	return m_parent; 
}
