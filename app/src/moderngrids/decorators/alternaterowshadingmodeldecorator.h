#pragma once

#include <QIdentityProxyModel>
#include <QColor>

class AlternateRowShadingModelDecorator : public QIdentityProxyModel
{
public:
    AlternateRowShadingModelDecorator(const QColor &colorA, const QColor &colorB, QObject *parent = nullptr);
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QColor m_colorA;
    QColor m_colorB;
};
