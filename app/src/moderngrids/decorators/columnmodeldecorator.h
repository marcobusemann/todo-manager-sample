#pragma once

#include <QIdentityProxyModel>

class ColumnModelDecorator : public QIdentityProxyModel
{
public:
    ColumnModelDecorator(int columns, QObject *parent = nullptr);
    virtual int	columnCount(const QModelIndex & = QModelIndex()) const override;
    virtual bool hasChildren(const QModelIndex &parent) const;

private:
    int m_columns;
};
