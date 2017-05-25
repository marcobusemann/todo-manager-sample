#pragma once

#include "qmgsourcemodelbuilder.h"
#include "qmgcommonmodelbuilder.h"

class QmgModelBuilder final
{
public:
	static QmgModelBuilder AModel(QObject *parent = nullptr);
	static QmgSourceModelBuilder AModelFor(const QmgObservableVariantListProxy &list, QObject *parent = nullptr);
	static QmgCommonModelBuilder AListModelFor(const QmgObservableVariantListProxy &list, QObject *parent = nullptr);

	template <class T>
	static QmgSourceModelBuilder AModelFor(QmgObservableList<T> &list, QObject *parent = nullptr)
	{
		return QmgSourceModelBuilder(QmgObservableVariantListProxy::from(list), parent);
	}

	template <class T>
	static QmgCommonModelBuilder AListModelFor(QmgObservableList<T> &list, QObject *parent = nullptr)
	{
		return QmgSourceModelBuilder(QmgObservableVariantListProxy::from(list), parent)
			.withColumns(1);
	}
	
	QmgSourceModelBuilder of(const QmgObservableVariantListProxy &list);

	template <class T>
	QmgSourceModelBuilder of(QmgObservableList<T> &list)
	{
		return of(QmgObservableVariantListProxy::from(list));
	}

private:
	QmgModelBuilder(QObject *parent);
	QObject * const m_parent;
};
