#pragma once

#include <QSharedPointer>
#include <QList>

#include <functional>

#include "qmgeventhandler.h"

/*
    Actually attached handlers will not be copied over to another instance. 
    Maybe this behavour will change in the future.
*/
template <class T>
class QmgObservableList final
{
public:
    static QmgObservableList<T> empty() { return QmgObservableList<T>(); }
    static QmgObservableList<T> fromList(const QList<T> &items) { return QmgObservableList<T>(items); }
    static QmgObservableList<T> fromList(const QmgObservableList<T> &items) { return QmgObservableList<T>(items); }

public:
    QmgObservableList() {}
    QmgObservableList(const QList<T> &list) : m_items(list) {}
    QmgObservableList(const QmgObservableList<T> &list) : m_items(list.m_items) {}

    QmgObservableList<T> &operator=(const QmgObservableList<T> &rhs)
    {   
        clear();
        append(rhs.m_items);
        return *this;
    }

    bool operator==(const QmgObservableList<T> &rhs)
    {
        return m_items == rhs.m_items;
    }

    QmgObservableList<T> &operator<<(const T &item)
    {
        append(item);
        return *this;
    }

    const T &at(int index) const
    { 
        return m_items.at(index); 
    }

    void append(const T &item)
    {
        int index = size();
        m_addBefore(index, item);
        m_items.append(item);
        m_addAfter(index, item);
        m_changeAfter();
    }

    void append(const QList<T> &items)
    {
        if (items.size() == 0) return;
        int index = size();
        m_addBatchBefore(index, items);
        m_items.append(items);
        m_addBatchAfter(index, items);
        m_changeAfter();
    }

    void append(const std::vector<T> &items)
    {
        if (items.size() == 0) return;
        QList<T> listitems;
        listitems.reserve(items.size());
        for (auto &item : items)
            listitems.append(item);
        append(listitems);
    }

    void insert(int index, const T &item)
    {
        m_addBefore(index, item);
        m_items.insert(index, item);
        m_addAfter(index, item);
        m_changeAfter();
    }

    void update(int index, const T &item)
    {
        m_updateBefore(index, item);
        m_items[index] = item;
        m_updateAfter(index, item);
        m_changeAfter();
    }

    void removeAt(int index)
    {
        const T item = at(index);
        m_removeBefore(index, item);
        m_items.removeAt(index);
        m_removeAfter(index, item);
        m_changeAfter();
    }

    template <class Compare>
    int indexOf(const T &value, int from = 0, Compare cmp = std::equal_to<T>()) const { 
        for (int i = from; i < m_items.size(); i++)
        {
            bool equals = cmp(m_items[i], value);
            if (equals)
                return i;
        }
        return -1;
    }

    int indexOf(const T &value, int from = 0) const {
        return indexOf(value, from, std::equal_to<T>());
    }

    int size() const { return m_items.size(); }
    bool isEmpty() const { return m_items.isEmpty(); }
    void reserve(int alloc) { m_items.reserve(alloc); }

    const QList<T> &toList() const { return m_items; }

    void clear()
    {
        bool hadElements = !isEmpty();
        m_clearBefore();
        m_items.clear();
        m_clearAfter();
        if (hadElements)
            m_changeAfter();
    }

    QmgEventHandler<void(int, const T &)> &beforeAdd() { return m_addBefore; }
    QmgEventHandler<void(int, const T &)> &afterAdd() { return m_addAfter; }

    QmgEventHandler<void(int, const QList<T> &)> &beforeAddBatch() { return m_addBatchBefore; }
    QmgEventHandler<void(int, const QList<T> &)> &afterAddBatch() { return m_addBatchAfter; }

    QmgEventHandler<void(int, const T &)> &beforeRemove() { return m_removeBefore; }
    QmgEventHandler<void(int, const T &)> &afterRemove() { return m_removeAfter; }

    QmgEventHandler<void(int, const T &)> &beforeUpdate() { return m_updateBefore; }
    QmgEventHandler<void(int, const T &)> &afterUpdate() { return m_updateAfter; }

    QmgEventHandler<void()> &beforeClear() { return m_clearBefore; }
    QmgEventHandler<void()> &afterClear() { return m_clearAfter; }

    QmgEventHandler<void()> &afterChange() { return m_changeAfter; }

private:
    QmgEventHandler<void(int, const T &)> m_addBefore;
    QmgEventHandler<void(int, const T &)> m_addAfter;

    QmgEventHandler<void(int, const QList<T> &)> m_addBatchBefore;
    QmgEventHandler<void(int, const QList<T> &)> m_addBatchAfter;

    QmgEventHandler<void(int, const T &)> m_removeBefore;
    QmgEventHandler<void(int, const T &)> m_removeAfter;

    QmgEventHandler<void(int, const T &)> m_updateBefore;
    QmgEventHandler<void(int, const T &)> m_updateAfter;

    QmgEventHandler<void()> m_clearBefore;
    QmgEventHandler<void()> m_clearAfter;

    QmgEventHandler<void()> m_changeAfter;

    QList<T> m_items;
};
