#pragma once

#include <QSharedPointer>

#include "qmgcolumnmodelbuilder.h"

class QmgColumnCollectionBuilder
{
public:
	typedef QSharedPointer<QmgColumnCollectionBuilder> Ptr;

	// Add filtering properties out
	// Apply model automatically base on the properties type
	template <class T, class TListType>
	static QmgColumnCollectionBuilder::Ptr from(QObject *parent = nullptr, std::function<bool(const QMetaProperty &prop)> filter = nullptr) {
		if (filter == nullptr)
			filter = [](const QMetaProperty &prop) {
			return prop.type() != QVariant::ByteArray;
		};

		QmgColumnCollectionBuilder::Ptr result(new QmgColumnCollectionBuilder());

		QMetaObject metaObject = T::staticMetaObject;
		for (int i = 0; i < metaObject.propertyCount(); i++) {
			QMetaProperty metaProperty = metaObject.property(i);

			if (!filter(metaProperty))
				continue;

			QString propertyName = QString("%1").arg(metaProperty.name());

			auto builder = ColumnModelBuilder::AProperty<TListType>(propertyName, parent)
				.withHeader(propertyName);

			result->addProperty(propertyName, builder);
		}

		return result;
	}

	template <class T, class TListType>
	static QmgColumnCollectionBuilder::Ptr from(QList<QString> properties, QObject *parent = nullptr) {
		QmgColumnCollectionBuilder::Ptr result(new QmgColumnCollectionBuilder());

		for (int i = 0; i < properties.length(); i++) {
			auto propertyName = properties[i];
			auto builder = ColumnModelBuilder::AProperty<TListType>(propertyName, parent)
				.withHeader(propertyName);
			result->addProperty(propertyName, builder);
		}

		return result;
	}

	void addColumn(const QmgColumnModelBuilder &column);
	void addProperty(const QString &propertyName, const QmgColumnModelBuilder &column);
	QmgColumnModelBuilder &getProperty(const QString &propertyName);
	const QList<QmgColumnModelBuilder> &toList() const;

private:
	QHash<QString, int> m_indexMap;
	QList<QmgColumnModelBuilder> m_builders;
};
