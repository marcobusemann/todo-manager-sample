#pragma once

#include <QSharedPointer>
#include <QList>
#include <QPair>

#include <functional>

class QmgScope
{
public:
    QmgScope(const std::function<void()> &detachCallback)
        : m_detachCallback(detachCallback)
    {
    }

    virtual ~QmgScope()
    {
        m_detachCallback();
    }

    void detach()
    {
        m_detachCallback();
        m_detachCallback = []() {};
    }

    void reset()
    {
        m_detachCallback = []() {};
    }

private:
    std::function<void()> m_detachCallback;
};

typedef QSharedPointer<QmgScope> QmgScopePtr;
typedef QWeakPointer<QmgScope>   QmgScopeWeakPtr;

/*
    This class allows adding event handlers which will be called later.
    Multiple handlers could be added. The order in which they will be executed is not specified.
    To prevent crashes by the event handler outloving other objects which registered callbacks, 
    a scope is used which handles the unregistration.
*/
template <class T>
class QmgEventHandler
{
public:
    QmgEventHandler()
        : m_counter(0)
    {}

    ~QmgEventHandler()
    {
        for (auto value : m_handlers)
        {
            auto weakPointer   = value.first;
            auto strongPointer = weakPointer.toStrongRef();
            if (strongPointer)
                strongPointer->reset();
        }
    }

    QmgScopePtr attach(const std::function<T> &handler)
    {
        auto key = m_counter++;
        auto scopePtr = QmgScopePtr(new QmgScope([this, key]() {
            m_handlers.remove(key);
        }));
        m_handlers[key] = qMakePair(scopePtr, handler);
        return scopePtr;
    }

    void detach(const QmgScopePtr &scope)
    {
        if (scope)
            scope->detach();
    }

    void attachUnsafe(const std::function<T> &handler)
    {
        m_handlers[m_counter++] = qMakePair(QmgScopeWeakPtr(), handler);
    }

    bool isEmpty() 
    {
        return m_handlers.isEmpty();
    }

    void operator()()
    {
        std::for_each(m_handlers.begin(), m_handlers.end(), [](auto value) {
            auto handler = value.second;
            handler();
        });
    }

    template <class K1>
    void operator()(const K1 &k1)
    {
        std::for_each(m_handlers.begin(), m_handlers.end(), [&k1](auto value) {
            auto handler = value.second;
            handler(k1);
        });
    }

    template <class K1, class K2>
    void operator()(const K1 &k1, const K2 &k2)
    {
        std::for_each(m_handlers.begin(), m_handlers.end(), [&k1, &k2](auto value) {
            auto handler = value.second;
            handler(k1, k2);
        });
    }

    template <class K1, class K2, class K3>
    void operator()(const K1 &k1, const K2 &k2, const K3 &k3)
    {
        std::for_each(m_handlers.begin(), m_handlers.end(), [&k1, &k2, &k3](auto value) {
            auto handler = value.second;
            handler(k1, k2, k3);
        });
    }

    // TODO: add variadic templates

    void operator+=(const std::function<T> &handler)
    {
        attachUnsafe(handler);
    }

private:
    unsigned short m_counter;

    typedef unsigned short HandlersKey;
    typedef QPair<QmgScopeWeakPtr, std::function<T>> HandlersValue;
    QHash<HandlersKey, HandlersValue> m_handlers;
};