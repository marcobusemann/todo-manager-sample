#pragma once

#include <QIdentityProxyModel>
#include <functional>

class QmgObjectDataModelDecorator : public QIdentityProxyModel
{
public:
    typedef std::function<QVariant(const QModelIndex &, int)> Handler;

public:
	QmgObjectDataModelDecorator(Handler handler, QObject *parent = nullptr);
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    template <class T>
    static QmgObjectDataModelDecorator *ofType(std::function<QVariant(int, const T &)> handler, QObject *parent)
    {
        auto adapterHandler = [handler](const QModelIndex &index, int) -> QVariant {
            auto item = index.data(Qt::UserRole).value<T>();
            return handler(index.column(), item);
        };
        return new QmgObjectDataModelDecorator(adapterHandler, parent);
    }

private:
    Handler m_handler;
};

class QmgObjectPropertyColumnModelDecorator : public QIdentityProxyModel
{
public:
    typedef std::function<QVariant(const QModelIndex &)> Handler;

public:
	QmgObjectPropertyColumnModelDecorator(int column, const QString &propertyName, Handler valueAdapter, QObject *parent = nullptr);
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    //virtual int	columnCount(const QModelIndex & = QModelIndex()) const override;

    template <class T>
    static QmgObjectPropertyColumnModelDecorator *ofType(int column, const QString &propertyName, QObject *parent)
    {
        auto valueAdapter = [propertyName](const QModelIndex &index) -> QVariant {
            auto object = index.data(Qt::UserRole).value<T>();
            return object->property(propertyName.toLocal8Bit().data());
        };
        return new QmgObjectPropertyColumnModelDecorator(column, propertyName, valueAdapter, parent);
    }

private:
    int m_column;
    Handler m_handler;
};
