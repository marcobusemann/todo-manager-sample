#pragma once

#include <QIdentityProxyModel>
#include <functional>

class ObjectDataModelDecorator : public QIdentityProxyModel
{
public:
    typedef std::function<QVariant(const QModelIndex &, int)> Handler;

public:
    ObjectDataModelDecorator(Handler handler, QObject *parent = nullptr);
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    template <class T>
    static ObjectDataModelDecorator *ofType(std::function<QVariant(int, const T &)> handler, QObject *parent)
    {
        auto adapterHandler = [handler](const QModelIndex &index, int) -> QVariant {
            auto item = index.data(Qt::UserRole).value<T>();
            return handler(index.column(), item);
        };
        return new ObjectDataModelDecorator(adapterHandler, parent);
    }

private:
    Handler m_handler;
};

class ObjectPropertyColumnModelDecorator : public QIdentityProxyModel
{
public:
    typedef std::function<QVariant(const QModelIndex &)> Handler;

public:
    ObjectPropertyColumnModelDecorator(int column, const QString &propertyName, Handler valueAdapter, QObject *parent = nullptr);
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    //virtual int	columnCount(const QModelIndex & = QModelIndex()) const override;

    template <class T>
    static ObjectPropertyColumnModelDecorator *ofType(int column, const QString &propertyName, QObject *parent)
    {
        auto valueAdapter = [propertyName](const QModelIndex &index) -> QVariant {
            auto object = index.data(Qt::UserRole).value<T>();
            return object->property(propertyName.toLocal8Bit().data());
        };
        return new ObjectPropertyColumnModelDecorator(column, propertyName, valueAdapter, parent);
    }

private:
    int m_column;
    Handler m_handler;
};
