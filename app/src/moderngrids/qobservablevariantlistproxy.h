#pragma once

#include "qobservablelist.h"

#include <QSharedPointer>
#include <QVariant>

#include <functional>
#include <memory>

class QObservableVariantListProxy
{
public:
    QObservableVariantListProxy() {}

    QObservableVariantListProxy(const QObservableVariantListProxy &rhs)
        : m_bindToList(rhs.m_bindToList)
        , m_atForwarder(rhs.m_atForwarder)
        , m_appendForwarder(rhs.m_appendForwarder)
        , m_insertForwarder(rhs.m_insertForwarder)
        , m_updateForwarder(rhs.m_updateForwarder)
        , m_removeForwarder(rhs.m_removeForwarder)
        , m_sizeForwarder(rhs.m_sizeForwarder)
        , m_isEmptyForwarder(rhs.m_isEmptyForwarder)
        , m_indexOfForwarder(rhs.m_indexOfForwarder)
        , m_clearForwarder(rhs.m_clearForwarder)
    {
        m_bindToList(*this);
    }

    QObservableVariantListProxy(const QObservableVariantListProxy &&rhs)
        : m_bindToList(std::move(rhs.m_bindToList))
        , m_atForwarder(std::move(rhs.m_atForwarder))
        , m_appendForwarder(std::move(rhs.m_appendForwarder))
        , m_insertForwarder(std::move(rhs.m_insertForwarder))
        , m_updateForwarder(std::move(rhs.m_updateForwarder))
        , m_removeForwarder(std::move(rhs.m_removeForwarder))
        , m_sizeForwarder(std::move(rhs.m_sizeForwarder))
        , m_isEmptyForwarder(std::move(rhs.m_isEmptyForwarder))
        , m_indexOfForwarder(std::move(rhs.m_indexOfForwarder))
        , m_clearForwarder(std::move(rhs.m_clearForwarder))
    {
        m_bindToList(*this);
    }

    void operator=(const QObservableVariantListProxy &rhs)
    {
        m_bindToList = rhs.m_bindToList;
        m_atForwarder = rhs.m_atForwarder;
        m_appendForwarder = rhs.m_appendForwarder;
        m_insertForwarder = rhs.m_insertForwarder;
        m_updateForwarder = rhs.m_updateForwarder;
        m_removeForwarder = rhs.m_removeForwarder;
        m_sizeForwarder = rhs.m_sizeForwarder;
        m_isEmptyForwarder = rhs.m_isEmptyForwarder;
        m_indexOfForwarder = rhs.m_indexOfForwarder;
        m_clearForwarder = rhs.m_clearForwarder;
        m_bindToList(*this);
    }

    template <class T>
    static QObservableVariantListProxy from(QObservableList<std::shared_ptr<T>> &list)
    {
        return from<std::shared_ptr<T>>(list[](const std::shared_ptr<T> &a, const std::shared_ptr<T> &b) {
            return a != nullptr && b != nullptr && *a == *b;
        });
    }

    template <class T>
    static QObservableVariantListProxy from(QObservableList<QSharedPointer<T>> &list)
    {
        return from<QSharedPointer<T>>(list, [](const QSharedPointer<T> &a, const QSharedPointer<T> &b) {
            return a != nullptr && b != nullptr && *a == *b;
        });
    }

    template <class T>
    static QObservableVariantListProxy from(QObservableList<T> &list)
    {
        return from(list, std::equal_to<T>());
    }

    template <class T, class Comperator>
    static QObservableVariantListProxy from(QObservableList<T> &list, Comperator comperator)
    {
        QObservableVariantListProxy result;

        result.m_atForwarder = [&list](int index) { 
            return QVariant::fromValue(list.at(index)); 
        };

        result.m_appendForwarder = [&list](const QVariant &item) {
            list.append(item.value<T>()); 
        };

        result.m_insertForwarder = [&list](int index, const QVariant &item) {
            list.insert(index, item.value<T>()); 
        };

        result.m_updateForwarder = [&list](int index, const QVariant &item) {
            list.update(index, item.value<T>());
        };

        result.m_removeForwarder = [&list](int index) { 
            list.removeAt(index); 
        };

        result.m_sizeForwarder = [&list]() -> int { 
            return list.size(); 
        };
        
        result.m_isEmptyForwarder = [&list]() -> bool { 
            return list.isEmpty(); 
        };

        result.m_indexOfForwarder = [&list, comperator](const QVariant &item, int startIndex) -> int {
            return list.indexOf(item.value<T>(), startIndex, comperator);
        };

        result.m_clearForwarder = [&list]() { 
            list.clear(); 
        };

        result.m_bindToList = [&list](QObservableVariantListProxy &proxy) {

            proxy.m_proxiedAddBefore = list.beforeAdd().attach([&proxy](int index, const T &item) -> void
            {
                proxy.m_addBefore(index, QVariant::fromValue(item));
            });

            proxy.m_proxiedAddBatchBefore = list.beforeAddBatch().attach([&proxy](int index, const QList<T> &items) -> void
            {
                proxy.m_addBatchBefore(index, QVariant::fromValue(items), items.size());
            });

            proxy.m_proxiedRemoveBefore = list.beforeRemove().attach([&proxy](int index, const T &item) -> void
            {
                proxy.m_removeBefore(index, QVariant::fromValue(item));
            });

            proxy.m_proxiedAddAfter = list.afterAdd().attach([&proxy](int index, const T &item) -> void
            {
                proxy.m_addAfter(index, QVariant::fromValue(item));
            });

            proxy.m_proxiedUpdateBefore = list.beforeUpdate().attach([&proxy](int index, const T &item) -> void
            {
                proxy.m_updateBefore(index, QVariant::fromValue(item));
            });

            proxy.m_proxiedUpdateAfter = list.afterUpdate().attach([&proxy](int index, const T &item) -> void
            {
                proxy.m_updateAfter(index, QVariant::fromValue(item));
            });

            proxy.m_proxiedAddBatchAfter = list.afterAddBatch().attach([&proxy](int index, const QList<T> &items) -> void
            {
                proxy.m_addBatchAfter(index, QVariant::fromValue(items), items.size());
            });

            proxy.m_proxiedRemoveAfter = list.afterRemove().attach([&proxy](int index, const T &item) -> void
            {
                proxy.m_removeAfter(index, QVariant::fromValue(item));
            });

            proxy.m_proxiedClearBefore = list.beforeClear().attach([&proxy]() -> void
            {
                proxy.m_clearBefore();
            });

            proxy.m_proxiedClearAfter = list.afterClear().attach([&proxy]() -> void
            {
                proxy.m_clearAfter();
            });

            proxy.m_proxiedChangeAfter = list.afterChange().attach([&proxy]() -> void {
                proxy.m_changeAfter();
            });
        };

        result.m_bindToList(result);

        return result;
    }

public:
    QVariant at(int index) const
    {
        return m_atForwarder(index);
    }

    void append(const QVariant &item)
    {
        m_appendForwarder(item);
    }

    void insert(int index, const QVariant &item)
    {
        m_insertForwarder(index, item);
    }

    void update(int index, const QVariant &item)
    {
        m_updateForwarder(index, item);
    }

    void removeAt(int index)
    {
        m_removeForwarder(index);
    }

    int indexOf(const QVariant &value, int from = 0) const 
    {
        return m_indexOfForwarder(value, from);
    }

    int size() const 
    { 
        return m_sizeForwarder(); 
    }
    
    bool isEmpty() const 
    { 
        return m_isEmptyForwarder(); 
    }

    void clear()
    {
        m_clearForwarder();
    }

    QObservableVariantListProxy &operator<<(const QVariant &item)
    {
        append(item);
        return *this;
    }

    QEventHandler<void(int, const QVariant &)> &beforeAdd() { return m_addBefore; }
    QEventHandler<void(int, const QVariant &)> &afterAdd() { return m_addAfter; }

    QEventHandler<void(int, const QVariant &, int)> &beforeAddBatch() { return m_addBatchBefore; }
    QEventHandler<void(int, const QVariant &, int)> &afterAddBatch() { return m_addBatchAfter; }

    QEventHandler<void(int, const QVariant &)> &beforeRemove() { return m_removeBefore; }
    QEventHandler<void(int, const QVariant &)> &afterRemove() { return m_removeAfter; }

    QEventHandler<void(int, const QVariant &)> &beforeUpdate() { return m_updateBefore; }
    QEventHandler<void(int, const QVariant &)> &afterUpdate() { return m_updateAfter; }

    QEventHandler<void()> &beforeClear() { return m_clearBefore; }
    QEventHandler<void()> &afterClear() { return m_clearAfter; }

    QEventHandler<void()> &afterChange() { return m_changeAfter; }

private:
    std::function<void(QObservableVariantListProxy &)> m_bindToList;

    std::function<QVariant(int)> m_atForwarder;
    std::function<void(const QVariant &)> m_appendForwarder;
    std::function<void(int, const QVariant &)> m_insertForwarder;
    std::function<void(int, const QVariant &)> m_updateForwarder;
    std::function<void(int)> m_removeForwarder;
    std::function<int()> m_sizeForwarder;
    std::function<bool()> m_isEmptyForwarder;
    std::function<int(const QVariant &, int)> m_indexOfForwarder;
    std::function<void()> m_clearForwarder;

    QEventHandler<void(int, const QVariant &)> m_addBefore;
    QEventHandler<void(int, const QVariant &)> m_addAfter;

    QEventHandler<void(int, const QVariant &, int)> m_addBatchBefore;
    QEventHandler<void(int, const QVariant &, int)> m_addBatchAfter;

    QEventHandler<void(int, const QVariant &)> m_removeBefore;
    QEventHandler<void(int, const QVariant &)> m_removeAfter;

    QEventHandler<void(int, const QVariant &)> m_updateBefore;
    QEventHandler<void(int, const QVariant &)> m_updateAfter;

    QEventHandler<void()> m_clearBefore;
    QEventHandler<void()> m_clearAfter;

    QEventHandler<void()> m_changeAfter;

    QScopePtr m_proxiedAddBefore;
    QScopePtr m_proxiedAddAfter;
    QScopePtr m_proxiedAddBatchBefore;
    QScopePtr m_proxiedAddBatchAfter;
    QScopePtr m_proxiedRemoveBefore;
    QScopePtr m_proxiedRemoveAfter;
    QScopePtr m_proxiedUpdateBefore;
    QScopePtr m_proxiedUpdateAfter;
    QScopePtr m_proxiedClearBefore;
    QScopePtr m_proxiedClearAfter;
    QScopePtr m_proxiedChangeAfter;
};
