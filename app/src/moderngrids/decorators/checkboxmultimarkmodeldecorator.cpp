#include "checkboxmultimarkmodeldecorator.h"

//-----------------------------------------------------------------------------
CheckboxMultiMarkModelDecorator::CheckboxMultiMarkModelDecorator(
    int column,
    UpdateCheckStateHandler updateHandler,
    IsCheckedHandler checkHandler,
    QObject *parent)
    : QIdentityProxyModel(parent)
    , m_column(column)
    , m_updateHandler(updateHandler)
    , m_checkHandler(checkHandler)
{
}

//-----------------------------------------------------------------------------
CheckboxMultiMarkModelDecorator::CheckboxMultiMarkModelDecorator(
    UpdateCheckStateHandler updateHandler,
    IsCheckedHandler checkHandler,
    QObject *parent)
    : QIdentityProxyModel(parent)
    , m_column(0)
    , m_updateHandler(updateHandler)
    , m_checkHandler(checkHandler)
{
}

//-----------------------------------------------------------------------------
QVariant CheckboxMultiMarkModelDecorator::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (role != Qt::CheckStateRole || index.column() != m_column) return __super::data(index, role);
    auto item = index.data(Qt::UserRole);
    return m_checkHandler(item) ? Qt::Checked : Qt::Unchecked;
}

//-----------------------------------------------------------------------------
bool CheckboxMultiMarkModelDecorator::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) return false;
    if (role != Qt::CheckStateRole || index.column() != m_column) return __super::setData(index, value, role);
    auto item = index.data(Qt::UserRole);
    m_updateHandler(item, value.toInt() == Qt::Checked);
    dataChanged(index, index);
    return true;
}

//-----------------------------------------------------------------------------
Qt::ItemFlags CheckboxMultiMarkModelDecorator::flags(const QModelIndex &index) const
{
    return __super::flags(index) | Qt::ItemIsUserCheckable;
}