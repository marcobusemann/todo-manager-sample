#pragma once

#include <functional>
#include <memory>

template <class T>
class ObservableSet final
{
public:
    typedef std::function<void(const T &, const ObservableSet<T> &)> Handler;
    typedef std::function<void(const ObservableSet<T> &)> ClearHandler;
    typedef std::shared_ptr<ObservableSet<T>> Ptr;

    Handler OnBeforeInsert;
    Handler OnBeforeRemove;
    Handler OnInsert;
    Handler OnRemove;

    ClearHandler OnBeforeClear;
    ClearHandler OnClear;

    static std::shared_ptr<ObservableSet<T>> empty()
    {
        return std::make_shared<ObservableSet<T>>();
    }

public:
    ObservableSet() {}
    virtual ~ObservableSet() {}

    void insert(const T &item)
    {
        if (OnBeforeInsert)
            OnBeforeInsert(item, *this);
        m_items.insert(item);
        if (OnInsert)
            OnInsert(item, *this);
    }

    void remove(const T &item)
    {
        if (OnBeforeRemove)
            OnBeforeRemove(item, *this);
        m_items.remove(item);
        if (OnRemove)
            OnRemove(item, *this);
    }

    void clear()
    {
        if (OnBeforeClear)
            OnBeforeClear(*this);
        m_items.clear();
        if (OnClear)
            OnClear(*this);
    }

    bool contains(const T &value) { return m_items.contains(value); }
    int size() const { return m_items.size(); }
    bool isEmpty() const { return m_items.isEmpty(); }

    QSet<T> toSet() const { return m_items; }

private:
    QSet<T> m_items;
};
