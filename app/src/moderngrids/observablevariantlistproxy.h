#pragma once

#include "iobservablevariantlist.h"
#include "observablelist.h"

#include <QSharedPointer>

#include <functional>
#include <memory>

class ObservableVariantListProxy : public IObservableVariantList
{
public:
    typedef std::shared_ptr<ObservableVariantListProxy> Ptr;

    Handler OnBeforeAdd;
    HandlerBatch OnBeforeAddBatch;
    Handler OnAdd;
    HandlerBatch OnAddBatch;

    Handler OnBeforeRemove;
    Handler OnRemove;

    ClearHandler OnBeforeClear;
    ClearHandler OnClear;

public:
    virtual ~ObservableVariantListProxy() {}

    template <class T>
    static ObservableVariantListProxy::Ptr from(const std::shared_ptr<ObservableList<std::shared_ptr<T>>> &list)
    {
        return from<std::shared_ptr<T>>(list, [](const std::shared_ptr<T> &a, const std::shared_ptr<T> &b) {
            return a != nullptr && b != nullptr && *a == *b;
        });
    }

    template <class T>
    static ObservableVariantListProxy::Ptr from(const std::shared_ptr<ObservableList<QSharedPointer<T>>> &list)
    {
        return from<QSharedPointer<T>>(list, [](const QSharedPointer<T> &a, const QSharedPointer<T> &b) {
            return a != nullptr && b != nullptr && *a == *b;
        });
    }

    template <class T, class Comperator>
    static ObservableVariantListProxy::Ptr from(const std::shared_ptr<ObservableList<T>> &list, Comperator comperator = std::equal_to<T>())
    {
        auto obj = std::make_shared<ObservableVariantListProxy>();
        obj->AtForwarder = [list](int index) { return QVariant::fromValue(list->at(index)); };
        obj->AppendForwarder = [list](const QVariant &item) -> void { list->append(item.value<T>()); };
        obj->InsertForwarder = [list](int index, const QVariant &item) -> void { list->insert(index, item.value<T>()); };
        obj->RemoveForwarder = [list](int index) -> void { list->removeAt(index); };
        obj->SizeForwarder = [list]() -> int { return list->size(); };
        obj->IsEmptyForwarder = [list]() -> bool { return list->isEmpty(); };
        obj->IndexOfForwarder = [list, comperator](const QVariant &item, int startIndex) -> int {
            return list->indexOf(item.value<T>(), startIndex, comperator);
        };
        obj->ClearForwarder = [list]() { list->clear(); };

        list->OnBeforeAdd = [obj](int index, const T &item) -> void
        {
            if (obj->OnBeforeAdd)
                obj->OnBeforeAdd(index, QVariant::fromValue(item));
        };
        list->OnBeforeAddBatch = [obj](int index, const QList<T> &items) -> void
        {
            if (obj->OnBeforeAddBatch)
                obj->OnBeforeAddBatch(index, QVariant::fromValue(items), items.size());
        };
        list->OnBeforeRemove = [obj](int index, const T &item) -> void
        {
            if (obj->OnBeforeRemove)
                obj->OnBeforeRemove(index, QVariant::fromValue(item));
        };

        list->OnAdd = [obj](int index, const T &item) -> void
        {
            if (obj->OnAdd)
                obj->OnAdd(index, QVariant::fromValue(item));
        };
        list->OnAddBatch = [obj](int index, const QList<T> &items) -> void
        {
            if (obj->OnAddBatch)
                obj->OnAddBatch(index, QVariant::fromValue(items), items.size());
        };
        list->OnRemove = [obj](int index, const T &item) -> void
        {
            if (obj->OnRemove)
                obj->OnRemove(index, QVariant::fromValue(item));
        };
        list->OnBeforeClear = [obj]() -> void
        {
            if (obj->OnBeforeClear)
                obj->OnBeforeClear();
        };
        list->OnClear = [obj]() -> void 
        {
            if (obj->OnClear)
                obj->OnClear();
        };

        return obj;
    }

public:
    ObservableVariantListProxy() {}

    virtual QVariant at(int index) override { return AtForwarder ? AtForwarder(index) : QVariant(); }
    virtual void append(const QVariant &item) override { if (AppendForwarder) AppendForwarder(item); }
    virtual void insert(int index, const QVariant &item) override { if (InsertForwarder) InsertForwarder(index, item); }
    virtual void removeAt(int index) override { if (RemoveForwarder) RemoveForwarder(index); }
    virtual int size() const override { return SizeForwarder(); }
    virtual bool isEmpty() const override { return IsEmptyForwarder(); }
    virtual int indexOf(const QVariant &item, int start = 0) { return IndexOfForwarder(item, start); };
    virtual void clear() override { ClearForwarder(); }

    virtual void setOnBeforeAdd(Handler handler) override { OnBeforeAdd = handler; }
    virtual void setOnBeforeAddBatch(HandlerBatch handler) override { OnBeforeAddBatch = handler; }
    virtual void setOnBeforeRemove(Handler handler) override { OnBeforeRemove = handler; }
    virtual void setOnAdd(Handler handler) override { OnAdd = handler; }
    virtual void setOnAddBatch(HandlerBatch handler) override { OnAddBatch = handler; }
    virtual void setOnRemove(Handler handler) override { OnRemove = handler; }
    virtual void setOnBeforeClear(ClearHandler handler) override { OnBeforeClear = handler; }
    virtual void setOnClear(ClearHandler handler) override { OnClear = handler; }

private:
    std::function<QVariant(int)> AtForwarder;
    std::function<void(const QVariant &)> AppendForwarder;
    std::function<void(int, const QVariant &)> InsertForwarder;
    std::function<void(int)> RemoveForwarder;
    std::function<int()> SizeForwarder;
    std::function<bool()> IsEmptyForwarder;
    std::function<int(const QVariant &, int)> IndexOfForwarder;
    std::function<void()> ClearForwarder;
};
