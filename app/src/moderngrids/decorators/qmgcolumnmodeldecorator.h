#pragma once

#include <QIdentityProxyModel>

class QmgColumnModelDecorator : public QIdentityProxyModel
{
public:
	QmgColumnModelDecorator(int columns, QObject *parent = nullptr);
    virtual int	columnCount(const QModelIndex & = QModelIndex()) const override;
    virtual bool hasChildren(const QModelIndex &parent) const;

private:
    int m_columns;
};
