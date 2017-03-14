#pragma once

#include <QVariant>

#include <functional>
#include <memory>

class IObservableVariantList
{
public:
    typedef std::function<void(int, const QVariant &)> Handler;
    typedef std::function<void(int, const QVariant &, int amount)> HandlerBatch;
    typedef std::shared_ptr<IObservableVariantList> Ptr;
    typedef std::function<void()> ClearHandler;

public:
    virtual ~IObservableVariantList() {}

    virtual void setOnBeforeAdd(Handler handler) = 0;
    virtual void setOnBeforeAddBatch(HandlerBatch handler) = 0;
    virtual void setOnAdd(Handler handler) = 0;
    virtual void setOnAddBatch(HandlerBatch handler) = 0;
    virtual void setOnBeforeRemove(Handler handler) = 0;
    virtual void setOnRemove(Handler handler) = 0;
    virtual void setOnBeforeClear(ClearHandler handler) = 0;
    virtual void setOnClear(ClearHandler handler) = 0;

    virtual QVariant at(int index) = 0;
    virtual void append(const QVariant &item) = 0;
    virtual void insert(int index, const QVariant &item) = 0;
    virtual void removeAt(int index) = 0;
    virtual int size() const = 0;
    virtual bool isEmpty() const = 0;
    virtual int indexOf(const QVariant &item, int start = 0) = 0;
    virtual void clear() = 0;
};
