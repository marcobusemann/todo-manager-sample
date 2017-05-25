#include "qmgdatamodelbuilder.h"
#include "qmgcommonmodelbuilder.h"

QmgDataModelBuilder::QmgDataModelBuilder(QAbstractItemModel *base, QObject *parent)
	: m_column(-1)
	, m_role(-1)
	, m_exclusions(QSet<int>() << Qt::UserRole)
	, m_base(base)
	, m_parent(parent)
{}

QmgDataModelBuilder &QmgDataModelBuilder::forColumn(int column)
{
	m_column = column;
	return *this;
}

QmgDataModelBuilder &QmgDataModelBuilder::forRole(int role)
{
	m_role = role;
	return *this;
}

QmgDataModelBuilder &QmgDataModelBuilder::withRoleExclusions(const QSet<int> &exclusions)
{
	m_exclusions = exclusions;
	return *this;
}

QmgCommonModelBuilder QmgDataModelBuilder::withHandler(QmgDataModelDecorator::Handler handler)
{
	QAbstractProxyModel *model = nullptr;
	if (m_column == -1 && m_role == -1)
		model = new QmgDataModelDecorator(m_exclusions, handler, m_parent);
	else if (m_role == -1)
		model = new QmgColumnBasedDataModelDecorator(m_column, m_exclusions, handler, m_parent);
	else if (m_column == -1)
		model = new QmgRoleBasedDataModelDecorator(m_role, handler, m_parent);
	else
		model = new QmgColumnAndRoleBasedDataModelDecorator(m_column, m_role, handler, m_parent);

	model->setSourceModel(m_base);
	return QmgCommonModelBuilder(model, m_parent);
}