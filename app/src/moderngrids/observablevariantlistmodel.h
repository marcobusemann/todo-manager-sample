#pragma once

#include "qobservablevariantlistproxy.h"

#include <QAbstractItemModel>
#include <functional>
#include <memory>

class ObservableVariantListModel : public QAbstractItemModel
{
public:
    ObservableVariantListModel(QObject *parent = nullptr);
    void setSource(const QObservableVariantListProxy &source);
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    virtual QModelIndex parent(const QModelIndex &) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex & = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QModelIndexList match(const QModelIndex &start, int role, const QVariant &value, int hits = 1, Qt::MatchFlags flags = Qt::MatchFlags(Qt::MatchStartsWith | Qt::MatchWrap)) const;

private:
    void linkTo(QObservableVariantListProxy &source);
    void unlinkFrom(QObservableVariantListProxy &source);

    QObservableVariantListProxy m_source;

    QScopePtr m_addBeforeScope;
    QScopePtr m_addAfterScope;
    QScopePtr m_addBatchBeforeScope;
    QScopePtr m_addBatchAfterScope;
    QScopePtr m_removeBeforeScope;
    QScopePtr m_removeAfterScope;
    QScopePtr m_clearBeforeScope;
    QScopePtr m_clearAfterScope;
};
