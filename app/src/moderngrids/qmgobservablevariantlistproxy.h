#pragma once

#include "qmgobservablelist.h"

#include <QSharedPointer>
#include <QVariant>

#include <functional>
#include <memory>

class QmgObservableVariantListProxy
{
public:
    QmgObservableVariantListProxy() {}

    QmgObservableVariantListProxy(const QmgObservableVariantListProxy &rhs)
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

    QmgObservableVariantListProxy(const QmgObservableVariantListProxy &&rhs)
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

    void operator=(const QmgObservableVariantListProxy &rhs)
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
    static QmgObservableVariantListProxy from(QmgObservableList<std::shared_ptr<T>> &list)
    {
        return from<std::shared_ptr<T>>(list[](const std::shared_ptr<T> &a, const std::shared_ptr<T> &b) {
            return a != nullptr && b != nullptr && *a == *b;
        });
    }

    template <class T>
    static QmgObservableVariantListProxy from(QmgObservableList<QSharedPointer<T>> &list)
    {
        return from<QSharedPointer<T>>(list, [](const QSharedPointer<T> &a, const QSharedPointer<T> &b) {
            return a != nullptr && b != nullptr && *a == *b;
        });
    }

    template <class T>
    static QmgObservableVariantListProxy from(QmgObservableList<T> &list)
    {
        return from(list, std::equal_to<T>());
    }

    template <class T, class Comperator>
    static QmgObservableVariantListProxy from(QmgObservableList<T> &list, Comperator comperator)
    {
        QmgObservableVariantListProxy result;

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

        result.m_bindToList = [&list](QmgObservableVariantListProxy &proxy) {

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

    QmgObservableVariantListProxy &operator<<(const QVariant &item)
    {
        append(item);
        return *this;
    }

	QmgEventHandler<void(int, const QVariant &)> &beforeAdd() { return m_addBefore; }
	QmgEventHandler<void(int, const QVariant &)> &afterAdd() { return m_addAfter; }

	QmgEventHandler<void(int, const QVariant &, int)> &beforeAddBatch() { return m_addBatchBefore; }
	QmgEventHandler<void(int, const QVariant &, int)> &afterAddBatch() { return m_addBatchAfter; }

	QmgEventHandler<void(int, const QVariant &)> &beforeRemove() { return m_removeBefore; }
	QmgEventHandler<void(int, const QVariant &)> &afterRemove() { return m_removeAfter; }

	QmgEventHandler<void(int, const QVariant &)> &beforeUpdate() { return m_updateBefore; }
	QmgEventHandler<void(int, const QVariant &)> &afterUpdate() { return m_updateAfter; }

	QmgEventHandler<void()> &beforeClear() { return m_clearBefore; }
	QmgEventHandler<void()> &afterClear() { return m_clearAfter; }

	QmgEventHandler<void()> &afterChange() { return m_changeAfter; }

private:
    std::function<void(QmgObservableVariantListProxy &)> m_bindToList;

    std::function<QVariant(int)> m_atForwarder;
    std::function<void(const QVariant &)> m_appendForwarder;
    std::function<void(int, const QVariant &)> m_insertForwarder;
    std::function<void(int, const QVariant &)> m_updateForwarder;
    std::function<void(int)> m_removeForwarder;
    std::function<int()> m_sizeForwarder;
    std::function<bool()> m_isEmptyForwarder;
    std::function<int(const QVariant &, int)> m_indexOfForwarder;
    std::function<void()> m_clearForwarder;

    QmgEventHandler<void(int, const QVariant &)> m_addBefore;
	QmgEventHandler<void(int, const QVariant &)> m_addAfter;

	QmgEventHandler<void(int, const QVariant &, int)> m_addBatchBefore;
	QmgEventHandler<void(int, const QVariant &, int)> m_addBatchAfter;

	QmgEventHandler<void(int, const QVariant &)> m_removeBefore;
	QmgEventHandler<void(int, const QVariant &)> m_removeAfter;

	QmgEventHandler<void(int, const QVariant &)> m_updateBefore;
	QmgEventHandler<void(int, const QVariant &)> m_updateAfter;

	QmgEventHandler<void()> m_clearBefore;
	QmgEventHandler<void()> m_clearAfter;

	QmgEventHandler<void()> m_changeAfter;

    QmgScopePtr m_proxiedAddBefore;
	QmgScopePtr m_proxiedAddAfter;
	QmgScopePtr m_proxiedAddBatchBefore;
	QmgScopePtr m_proxiedAddBatchAfter;
	QmgScopePtr m_proxiedRemoveBefore;
	QmgScopePtr m_proxiedRemoveAfter;
	QmgScopePtr m_proxiedUpdateBefore;
	QmgScopePtr m_proxiedUpdateAfter;
	QmgScopePtr m_proxiedClearBefore;
	QmgScopePtr m_proxiedClearAfter;
	QmgScopePtr m_proxiedChangeAfter;
};
