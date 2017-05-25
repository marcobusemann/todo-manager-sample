#include "qmgalternaterowshadingmodeldecorator.h"

QmgAlternateRowShadingModelDecorator::QmgAlternateRowShadingModelDecorator(const QColor &colorA, const QColor &colorB, QObject *parent)
    : QIdentityProxyModel(parent)
    , m_colorA(colorA)
    , m_colorB(colorB)
{
}

QVariant QmgAlternateRowShadingModelDecorator::data(const QModelIndex &index, int role) const
{
    if (role != Qt::BackgroundRole) return __super::data(index, role);
    QVariant rawResult = QIdentityProxyModel::data(index, role);
    if (rawResult.isValid()) return rawResult;
    return index.isValid() ? (index.row() % 2 == 0 ? m_colorA : m_colorB) : QVariant();
}