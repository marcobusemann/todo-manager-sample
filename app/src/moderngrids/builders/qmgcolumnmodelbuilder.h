#pragma once

#include <QIdentityProxyModel>
#include <QIcon>

#include <functional>

#include "../decorators/qmgobjectdatamodeldecorator.h"

class QmgColumnModelBuilder {
public:
	typedef std::function<QIdentityProxyModel*(int)> ModelFactory;

	template <class T>
	static QmgColumnModelBuilder AProperty(const QString &propertyName, QObject *parent = nullptr) {
		QmgColumnModelBuilder tmp;
		tmp.m_parent = parent;
		tmp.m_factories.append([propertyName, parent](int column) {
			return QmgObjectPropertyColumnModelDecorator::ofType<T>(column, propertyName, parent);
		});
		return tmp;
	}

	static QmgColumnModelBuilder ACustom(QObject *parent = nullptr);
	QmgColumnModelBuilder &withHeader(const QString &headerText, QIcon icon = QIcon(), Qt::Orientation orientation = Qt::Horizontal);
	QmgColumnModelBuilder &withMapData(int role, std::function<QVariant(const QModelIndex &index, const QVariant &rawValue)> mapHandler);
	QmgColumnModelBuilder &withData(int role, std::function<QVariant(const QModelIndex &index)> mapHandler);
	QmgColumnModelBuilder &withConstData(int role, std::function<QVariant()> mapHandler);
	QmgColumnModelBuilder &withConstData(int role, const QVariant &value);

	template <class T>
	QmgColumnModelBuilder &withModel() {
		auto parent = this->getParent();
		m_factories.append([parent](int column) {
			return new T(column, parent);
		});
		return *this;
	}

	QmgColumnModelBuilder &withData(std::function<QVariant(const QModelIndex &index, int role, std::function<QVariant()> rawValueGetter)> mapHandler);
	QAbstractItemModel *build(int column, QAbstractItemModel *startModel = nullptr) const;
	QObject *getParent() const;

private:
	QList<ModelFactory> m_factories;
	QObject *m_parent;
};