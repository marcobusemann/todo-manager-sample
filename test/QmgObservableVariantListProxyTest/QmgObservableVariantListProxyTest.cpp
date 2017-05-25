#include <QtTest>

#include <moderngrids\qmgobservablevariantlistproxy.h>

class QmgObservableVariantListProxyTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void shouldWorkAfterEquation()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);

        int callbackOneCallCount = 0;
        proxy.afterAdd() += [&callbackOneCallCount](int index, const QVariant &value) {
            callbackOneCallCount++;
            if (index == 0)
                QCOMPARE(value, QVariant(10));
            else if (index == 1)
                QCOMPARE(value, QVariant(15));
        };

        proxy.append(10);

        auto proxyCopy = proxy;

        int callbackTwoCallCount = 0;
        proxyCopy.afterAdd() += [&callbackTwoCallCount](int index, const QVariant &value) {
            callbackTwoCallCount++;
            QCOMPARE(value, QVariant(15));
        };

        proxyCopy.append(15);

        QCOMPARE(callbackOneCallCount, 2);
        QCOMPARE(callbackTwoCallCount, 1);
        QCOMPARE(list.size(), 2);
    }

    void size_listIsEmpty_zero()
    {
        auto list  = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);

        QCOMPARE(list.size(), 0);
        QCOMPARE(proxy.size(), 0);
    }

    void size_listHasOneElement_one()
    {
        auto list  = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);

        proxy.append(10);

        QCOMPARE(list.size(), 1);
        QCOMPARE(proxy.size(), 1);
    }

    void isEmpty_listIsEmpty_true()
    {
        auto list  = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);

        QCOMPARE(list.isEmpty(), true);
        QCOMPARE(proxy.isEmpty(), true);
    }

    void isEmpty_listHasOneElement_false()
    {
        auto list  = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);

        proxy.append(10);

        QCOMPARE(list.isEmpty(), false);
        QCOMPARE(proxy.isEmpty(), false);
    }

    void clear_listIsEmpty_listIsEmpty()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);

        proxy.clear();

        QCOMPARE(proxy.isEmpty(), true);
    }

    void clear_listHasOneElement_listIsEmpty()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);
        proxy.append(10);

        proxy.clear();

        QCOMPARE(proxy.isEmpty(), true);
    }

    void append_listIsEmpty_oneElement()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);

        proxy.append(10);

        QCOMPARE(proxy.size(), 1);
        QCOMPARE(proxy.at(0), QVariant(10));
    }

    void insert_listIsEmpty_oneElement()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);

        proxy.insert(0, 10);

        QCOMPARE(proxy.size(), 1);
        QCOMPARE(proxy.at(0), QVariant(10));
    }

    void update_listHasOnElement_oneElementHasChanged()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);
        proxy.append(10);

        proxy.update(0, 15);

        QCOMPARE(proxy.at(0), QVariant(15));
    }

    void removeAt_oneItem_zeroItems()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);
        proxy.append(10);

        proxy.removeAt(0);

        QCOMPARE(proxy.size(), 0);
    }

    void indexOf_listIsEmpty_minusOne()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);

        QCOMPARE(proxy.indexOf(10), -1);
    }

    void indexOf_listHasOneElement_indexOfThatElement()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);
        proxy.append(10);

        QCOMPARE(proxy.indexOf(10), 0);
    }

    void indexOf_listHasThreeElementsSearchTheMiddleOne_indexOfTheMiddleOne()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);
        proxy.append(5);
        proxy.append(10);
        proxy.append(15);

        QCOMPARE(proxy.indexOf(QVariant(10)), 1);
    }

    void indexOf_listHasThreeElementsSearchedIsTwoTimesPresentStartAtIndexOfPlusOne_IndexOfSecond()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);
        proxy.append(10);
        proxy.append(5);
        proxy.append(10);

        QCOMPARE(proxy.indexOf(10, 1), 2);
    }

    void empty_emptyList()
    {
        auto list = QmgObservableList<int>::empty();
        auto proxy = QmgObservableVariantListProxy::from(list);

        QCOMPARE(proxy.size(), 0);
    }

    void fromList_QListWithTwoElements_ObservableListWithTheSameElements()
    {
        auto sourceList = QList<int>() << 5 << 10;
        auto list = QmgObservableList<int>::fromList(sourceList);
        auto proxy = QmgObservableVariantListProxy::from(list);

        QCOMPARE(proxy.at(0), QVariant(5));
        QCOMPARE(proxy.at(1), QVariant(10));
    }

    void fromList_ObservableLIstWithTwoElements_ObservableListWithTheSameElements()
    {
        auto sourceList = QmgObservableList<int>::empty();
        sourceList.append(5);
        sourceList.append(10);

        auto list = QmgObservableList<int>::fromList(sourceList);
        auto proxy = QmgObservableVariantListProxy::from(list);

        QCOMPARE(proxy.at(0), QVariant(5));
        QCOMPARE(proxy.at(1), QVariant(10));
    }

    void operatorInputStream_listIsEmpty_oneElement()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);

        proxy << 10;

        QCOMPARE(proxy.size(), 1);
        QCOMPARE(proxy.at(0), QVariant(10));
    }

    void beforeAddForAppend_listIsEmpty_handlerIsCalledBeforeListIsIncreased()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);

        bool handlerHasBeenCalled = false;
        proxy.beforeAdd() += [&handlerHasBeenCalled, &proxy](int index, const QVariant &value) {
            handlerHasBeenCalled = true;
            QCOMPARE(value, QVariant(10));
            QCOMPARE(proxy.isEmpty(), true);
        };

        proxy.append(10);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void afterAddForAppend_listIsEmpty_handlerIsCalledAfterListHasBeenIncreased()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);

        bool handlerHasBeenCalled = false;
        proxy.afterAdd() += [&handlerHasBeenCalled, &proxy](int index, const QVariant &value) {
            handlerHasBeenCalled = true;
            QCOMPARE(value, QVariant(10));
            QCOMPARE(proxy.isEmpty(), false);
        };

        proxy.append(10);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void beforeAddForInsert_listIsEmpty_handlerIsCalledBeforeListIsIncreased()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);

        bool handlerHasBeenCalled = false;
        proxy.beforeAdd() += [&handlerHasBeenCalled, &proxy](int index, const QVariant &value) {
            handlerHasBeenCalled = true;
            QCOMPARE(index, 0);
            QCOMPARE(value, QVariant(10));
            QCOMPARE(proxy.isEmpty(), true);
        };

        proxy.insert(0, 10);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void afterAddForInsert_listIsEmpty_handlerIsCalledAfterListHasBeenIncreased()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);

        bool handlerHasBeenCalled = false;
        proxy.afterAdd() += [&handlerHasBeenCalled, &proxy](int index, const QVariant &value) {
            handlerHasBeenCalled = true;
            QCOMPARE(index, 0);
            QCOMPARE(value, QVariant(10));
            QCOMPARE(proxy.isEmpty(), false);
        };

        proxy.insert(0, 10);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void beforeClear_listHasOneElement_handlerIsCalledBeforeListIsCleared()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);
        proxy.append(10);

        bool handlerHasBeenCalled = false;
        proxy.beforeClear() += [&handlerHasBeenCalled, &proxy]() {
            handlerHasBeenCalled = true;
            QCOMPARE(proxy.isEmpty(), false);
        };

        proxy.clear();

        QCOMPARE(proxy.isEmpty(), true);
        QCOMPARE(handlerHasBeenCalled, true);
    }

    void afterClear_listHasOneElement_handlerIsCalledAfterListIsCleared()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);
        proxy.append(10);

        bool handlerHasBeenCalled = false;
        proxy.afterClear() += [&handlerHasBeenCalled, &proxy]() {
            handlerHasBeenCalled = true;
            QCOMPARE(proxy.isEmpty(), true);
        };

        proxy.clear();

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void beforeRemove_listHasOneElement_handlerIsCalledBeforeListIsDecreased()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);
        proxy.append(10);

        bool handlerHasBeenCalled = false;
        proxy.beforeRemove() += [&handlerHasBeenCalled, &proxy](int index, const QVariant &value) {
            handlerHasBeenCalled = true;
            QCOMPARE(value, QVariant(10));
            QCOMPARE(proxy.isEmpty(), false);
        };

        proxy.removeAt(0);

        QCOMPARE(proxy.isEmpty(), true);
        QCOMPARE(handlerHasBeenCalled, true);
    }

    void afterRemove_listHasOneElement_handlerIsCalledAfterListIsDecreased()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);
        proxy.append(10);

        bool handlerHasBeenCalled = false;
        proxy.afterRemove() += [&handlerHasBeenCalled, &proxy](int index, const QVariant &value) {
            handlerHasBeenCalled = true;
            QCOMPARE(value, QVariant(10));
            QCOMPARE(proxy.isEmpty(), true);
        };

        proxy.removeAt(0);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void beforeUpdate_listHasOneElement_handlerIsCalledBeforeListIsUpdated()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);
        proxy.append(10);

        bool handlerHasBeenCalled = false;
        proxy.beforeUpdate() += [&handlerHasBeenCalled, &proxy](int index, const QVariant &value) {
            handlerHasBeenCalled = true;
            QCOMPARE(value, QVariant(15));
        };

        proxy.update(0, 15);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void afterUpdate_listHasOneElement_handlerIsCalledAfterListIsUpdated()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);
        proxy.append(10);

        bool handlerHasBeenCalled = false;
        proxy.afterUpdate() += [&handlerHasBeenCalled, &proxy](int index, const QVariant &value) {
            handlerHasBeenCalled = true;
            QCOMPARE(value, QVariant(15));
        };

        proxy.update(0, 15);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void beforeAddBatch_listIsEmpty_handlerIsCalledBeforeListIsIncreased()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);

        bool handlerHasBeenCalled = false;
        proxy.beforeAddBatch() += [&handlerHasBeenCalled, &proxy](int index, const QVariant &value, int size) {
            handlerHasBeenCalled = true;
            QCOMPARE(size, 2);
            QCOMPARE(proxy.isEmpty(), true);
        };

        list.append(QList<int>() << 10 << 15);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void afterAddBatch_listIsEmpty_handlerIsCalledAfterListIsIncreased()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);

        bool handlerHasBeenCalled = false;
        proxy.afterAddBatch() += [&handlerHasBeenCalled, &proxy](int index, const QVariant &value, int size) {
            handlerHasBeenCalled = true;
            QCOMPARE(size, 2);
            QCOMPARE(proxy.isEmpty(), false);
        };

        list.append(QList<int>() << 10 << 15);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void beforeAddBatchForStdVector_listIsEmpty_handlerIsCalledBeforeListIsIncreased()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);

        bool handlerHasBeenCalled = false;
        proxy.beforeAddBatch() += [&handlerHasBeenCalled, &proxy](int index, const QVariant &value, int size) {
            handlerHasBeenCalled = true;
            QCOMPARE(size, 2);
            QCOMPARE(proxy.isEmpty(), true);
        };

        std::vector<int> tmp;
        tmp.push_back(10);
        tmp.push_back(15);

        list.append(tmp);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void afterAddBatchForStdVector_listIsEmpty_handlerIsCalledAfterListIsIncreased()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);

        bool handlerHasBeenCalled = false;
        proxy.afterAddBatch() += [&handlerHasBeenCalled, &proxy](int index, const QVariant &value, int size) {
            handlerHasBeenCalled = true;
            QCOMPARE(size, 2);
            QCOMPARE(proxy.isEmpty(), false);
        };

        std::vector<int> tmp;
        tmp.push_back(10);
        tmp.push_back(15);

        list.append(tmp);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void afterChangeForAppend_listIsEmpty_handlerIsCalledAfterListHasChanged()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);

        bool handlerHasBeenCalled = false;
        proxy.afterChange() += [&handlerHasBeenCalled]() {
            handlerHasBeenCalled = true;
        };

        proxy.append(10);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void afterChangeForAppendBatch_listIsEmpty_handlerIsCalledAfterListHasChanged()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);

        bool handlerHasBeenCalled = false;
        proxy.afterChange() += [&handlerHasBeenCalled]() {
            handlerHasBeenCalled = true;
        };

        list.append(QList<int>() << 10 << 15);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void afterChangeForAppendBatchStdVector_listIsEmpty_handlerIsCalledAfterListHasChanged()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);

        bool handlerHasBeenCalled = false;
        proxy.afterChange() += [&handlerHasBeenCalled]() {
            handlerHasBeenCalled = true;
        };

        std::vector<int> tmp;
        tmp.push_back(10);
        tmp.push_back(15);

        list.append(tmp);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void afterChangeForInsert_listIsEmpty_handlerIsCalledAfterListHasChanged()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);

        bool handlerHasBeenCalled = false;
        proxy.afterChange() += [&handlerHasBeenCalled]() {
            handlerHasBeenCalled = true;
        };

        proxy.insert(0, 10);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void afterChangeForUpdate_listIsEmpty_handlerIsCalledAfterListHasChanged()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);
        proxy.append(10);

        bool handlerHasBeenCalled = false;
        proxy.afterChange() += [&handlerHasBeenCalled]() {
            handlerHasBeenCalled = true;
        };

        proxy.update(0, 15);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void afterChangeForRemove_listHasOneElement_handlerIsCalledAfterListHasChanged()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);
        proxy.append(10);

        bool handlerHasBeenCalled = false;
        proxy.afterChange() += [&handlerHasBeenCalled]() {
            handlerHasBeenCalled = true;
        };

        proxy.removeAt(0);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void afterChangeForClear_listHasOneElement_handlerIsCalledAfterListHasChanged()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);
        proxy.append(10);

        bool handlerHasBeenCalled = false;
        list.afterChange() += [&handlerHasBeenCalled]() {
            handlerHasBeenCalled = true;
        };

        proxy.clear();

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void afterChangeForClear_listHasNoElement_handlerIsNotCalledAfterListHasChanged()
    {
        auto list = QmgObservableList<int>();
        auto proxy = QmgObservableVariantListProxy::from(list);

        bool handlerHasBeenCalled = false;
        proxy.afterChange() += [&handlerHasBeenCalled]() {
            handlerHasBeenCalled = true;
        };

        proxy.clear();

        QCOMPARE(handlerHasBeenCalled, false);
    }
};

QTEST_APPLESS_MAIN(QmgObservableVariantListProxyTest)

#include "QmgObservableVariantListProxyTest.moc"
