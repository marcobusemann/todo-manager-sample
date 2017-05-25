#pragma once

#include <QAbstractItemModel>

#include "../decorators/qmgdatamodeldecorator.h"

class QmgCommonModelBuilder;

class QmgDataModelBuilder final
{
public:
	QmgDataModelBuilder(QAbstractItemModel *base, QObject *parent);
	QmgDataModelBuilder &forColumn(int column);
	QmgDataModelBuilder &forRole(int role);
	QmgDataModelBuilder &withRoleExclusions(const QSet<int> &exclusions);
	QmgCommonModelBuilder withHandler(QmgDataModelDecorator::Handler handler);

private:
	QObject * const m_parent;
	int m_column;
	int m_role;
	QSet<int> m_exclusions;
	QAbstractItemModel *m_base;
};