#include "qmgheaderdatamodelbuilder.h"
#include "qmgcommonmodelbuilder.h"

QmgHeaderDataModelBuilder::QmgHeaderDataModelBuilder(QAbstractItemModel *base, QObject *parent)
	: m_section(-1)
	, m_role(-1)
	, m_orientation(Qt::Horizontal)
	, m_base(base)
	, m_parent(parent)
{}

QmgHeaderDataModelBuilder &QmgHeaderDataModelBuilder::forSection(int section)
{
	m_section = section;
	return *this;
}

QmgHeaderDataModelBuilder &QmgHeaderDataModelBuilder::forRole(int role)
{
	m_role = role;
	return *this;
}

QmgHeaderDataModelBuilder &QmgHeaderDataModelBuilder::withOrientation(Qt::Orientation orientation)
{
	m_orientation = orientation;
	return *this;
}

QmgCommonModelBuilder QmgHeaderDataModelBuilder::withHandler(QmgHeaderDataModelDecorator::Handler handler)
{
	QAbstractProxyModel *model = nullptr;
	if (m_section == -1 && m_role == -1)
		model = new QmgHeaderDataModelDecorator(m_orientation, handler, m_parent);
	else if (m_role == -1)
		model = new QmgSectionBasedHeaderDataModelDecorator(m_section, m_orientation, handler, m_parent);
	else if (m_section == -1)
		model = new QmgRoleBasedHeaderDataModelDecorator(m_role, m_orientation, handler, m_parent);
	else
		model = new QmgSectionAndRoleBasedHeaderDataModelDecorator(m_section, m_role, m_orientation, handler, m_parent);

	model->setSourceModel(m_base);
	return QmgCommonModelBuilder(model, m_parent);
}