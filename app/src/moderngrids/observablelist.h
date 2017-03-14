#pragma once

#include <functional>
#include <memory>

#include <QList>

template <class T>
class ObservableList final
{
public:
    typedef std::function<void(int, const T &)> Handler;
    typedef std::function<void(int, const QList<T> &)> HandlerBatch;
    typedef std::function<void()> ClearHandler;
    typedef std::function<void()> ChangeHandler;
    typedef std::shared_ptr<ObservableList<T>> Ptr;

    Handler OnBeforeAdd;
    HandlerBatch OnBeforeAddBatch;
    Handler OnBeforeRemove;
    Handler OnAdd;
    HandlerBatch OnAddBatch;
    Handler OnRemove;
    ClearHandler OnBeforeClear;
    ClearHandler OnClear;

    ChangeHandler OnChange;

    static std::shared_ptr<ObservableList<T>> empty()
    {
        return std::make_shared<ObservableList<T>>();
    }

    static std::shared_ptr<ObservableList<T>> fromList(const QList<T> &aItems)
    {
        return std::make_shared<ObservableList<T>>(aItems);
    }

    static std::shared_ptr<ObservableList<T>> fromList(const std::shared_ptr<ObservableList<T>> &aItems)
    {
        return std::make_shared<ObservableList<T>>(aItems);
    }

public:
    ObservableList() {}
    ObservableList(const QList<T> &aList)
        : m_items(aList)
    {
    }
    ObservableList(const std::shared_ptr<ObservableList<T>> &aList)
        : m_items(aList->m_items)
    {
    }

    ObservableList<T> &operator=(const ObservableList<T> &rhs)
    {   
        clear();
        append(rhs.m_items);
        return *this;
    }

    bool operator==(const ObservableList<T> &rhs)
    {
        return m_items == rhs.m_items;
    }

    const T &at(int index) { return m_items.at(index); }

    void append(const T &item)
    {
        int index = size();
        if (OnBeforeAdd)
            OnBeforeAdd(index, item);
        m_items.append(item);
        if (OnAdd)
            OnAdd(index, item);
        if (OnChange)
            OnChange();
    }

    void append(const QList<T> &items)
    {
        if (items.size() == 0) return;
        int index = size();
        if (OnBeforeAddBatch)
            OnBeforeAddBatch(index, items);
        m_items.append(items);
        if (OnAddBatch)
            OnAddBatch(index, items);
        if (OnChange)
            OnChange();
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
        if (OnBeforeAdd)
            OnBeforeAdd(index, item);
        m_items.insert(index, item);
        if (OnAdd)
            OnAdd(index, item);
        if (OnChange)
            OnChange();
    }

    void removeAt(int index)
    {
        const T item = at(index);
        if (OnBeforeRemove)
            OnBeforeRemove(index, item);
        m_items.removeAt(index);
        if (OnRemove)
            OnRemove(index, item);
        if (OnChange)
            OnChange();
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

    QList<T> toList() const { return m_items; }

    void clear()
    {
        if (OnBeforeClear)
            OnBeforeClear();
        m_items.clear();
        if (OnClear)
            OnClear();
        if (OnChange)
            OnChange();
    }

private:
    QList<T> m_items;
};
