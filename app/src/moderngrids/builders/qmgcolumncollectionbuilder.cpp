#include "qmgcolumncollectionbuilder.h"

void QmgColumnCollectionBuilder::addColumn(const QmgColumnModelBuilder &column) 
{
	m_builders.append(column);
}

void QmgColumnCollectionBuilder::addProperty(const QString &propertyName, const QmgColumnModelBuilder &column)
{
	m_indexMap[propertyName] = m_builders.size();
	m_builders.append(column);
}

QmgColumnModelBuilder &QmgColumnCollectionBuilder::getProperty(const QString &propertyName)
{
	return m_builders[m_indexMap[propertyName]];
}

const QList<QmgColumnModelBuilder> &QmgColumnCollectionBuilder::toList() const
{
	return m_builders;
}
