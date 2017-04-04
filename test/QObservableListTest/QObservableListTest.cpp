#include <QtTest>

#include <moderngrids\qobservablelist.h>

class QObservableListTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void size_listIsEmpty_zero()
    {
        QObservableList<int> list;

        QCOMPARE(list.size(), 0);
    }

    void size_listHasOneElement_one()
    {
        QObservableList<int> list;
        list.append(10);

        QCOMPARE(list.size(), 1);
    }

    void isEmpty_listIsEmpty_true()
    {
        QObservableList<int> list;

        QCOMPARE(list.isEmpty(), true);
    }

    void isEmpty_listHasOneElement_false()
    {
        QObservableList<int> list;
        list.append(10);

        QCOMPARE(list.isEmpty(), false);
    }

    void clear_listIsEmpty_listIsEmpty()
    {
        QObservableList<int> list;

        list.clear();

        QCOMPARE(list.isEmpty(), true);
    }

    void clear_listHasOneElement_listIsEmpty()
    {
        QObservableList<int> list;
        list.append(10);

        list.clear();

        QCOMPARE(list.isEmpty(), true);
    }

    void append_listIsEmpty_oneElement()
    {
        QObservableList<int> list;

        list.append(10);

        QCOMPARE(list.size(), 1);
        QCOMPARE(list.at(0), 10);
    }

    void insert_listIsEmpty_oneElement()
    {
        QObservableList<int> list;

        list.insert(0, 10);

        QCOMPARE(list.size(), 1);
        QCOMPARE(list.at(0), 10);
    }

    void removeAt_oneItem_zeroItems()
    {
        QObservableList<int> list;
        list.append(10);

        list.removeAt(0);

        QCOMPARE(list.size(), 0);
    }

    void indexOf_listIsEmpty_minusOne()
    {
        QObservableList<int> list;

        QCOMPARE(list.indexOf(10), -1);
    }

    void indexOf_listHasOneElement_indexOfThatElement()
    {
        QObservableList<int> list;
        list.append(10);

        QCOMPARE(list.indexOf(10), 0);
    }

    void indexOf_listHasThreeElementsSearchTheMiddleOne_indexOfTheMiddleOne()
    {
        QObservableList<int> list;
        list.append(5);
        list.append(10);
        list.append(15);

        QCOMPARE(list.indexOf(10), 1);
    }

    void indexOf_listHasThreeElementsSearchedIsTwoTimesPresentStartAtIndexOfPlusOne_IndexOfSecond()
    {
        QObservableList<int> list;
        list.append(10);
        list.append(5);
        list.append(10);

        QCOMPARE(list.indexOf(10, 1), 2);
    }

    void empty_emptyList()
    {
        auto list = QObservableList<int>::empty();

        QCOMPARE(list.size(), 0);
    }

    void fromList_QListWithTwoElements_ObservableListWithTheSameElements()
    {
        auto sourceList = QList<int>() << 5 << 10;

        auto list = QObservableList<int>::fromList(sourceList);

        QCOMPARE(list.at(0), 5);
        QCOMPARE(list.at(1), 10);
    }

    void fromList_ObservableLIstWithTwoElements_ObservableListWithTheSameElements()
    {
        auto sourceList = QObservableList<int>::empty();
        sourceList.append(5);
        sourceList.append(10);

        auto list = QObservableList<int>::fromList(sourceList);

        QCOMPARE(list.at(0), 5);
        QCOMPARE(list.at(1), 10);
    }

    void operatorInputStream_listIsEmpty_oneElement()
    {
        QObservableList<int> list;

        list << 10;

        QCOMPARE(list.size(), 1);
        QCOMPARE(list.at(0), 10);
    }

    void beforeAddForAppend_listIsEmpty_handlerIsCalledBeforeListIsIncreased()
    {
        QObservableList<int> list;
       
        bool handlerHasBeenCalled = false;
        list.beforeAdd() += [&handlerHasBeenCalled, &list](int index, const int &value) {
            handlerHasBeenCalled = true;
            QCOMPARE(value, 10);
            QCOMPARE(list.isEmpty(), true);
        };

        list.append(10);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void afterAddForAppend_listIsEmpty_handlerIsCalledAfterListHasBeenIncreased()
    {
        QObservableList<int> list;

        bool handlerHasBeenCalled = false;
        list.afterAdd() += [&handlerHasBeenCalled, &list](int index, const int &value) {
            handlerHasBeenCalled = true;
            QCOMPARE(value, 10);
            QCOMPARE(list.isEmpty(), false);
        };

        list.append(10);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void beforeAddForInsert_listIsEmpty_handlerIsCalledBeforeListIsIncreased()
    {
        QObservableList<int> list;

        bool handlerHasBeenCalled = false;
        list.beforeAdd() += [&handlerHasBeenCalled, &list](int index, const int &value) {
            handlerHasBeenCalled = true;
            QCOMPARE(index, 0);
            QCOMPARE(value, 10);
            QCOMPARE(list.isEmpty(), true);
        };

        list.insert(0, 10);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void afterAddForInsert_listIsEmpty_handlerIsCalledAfterListHasBeenIncreased()
    {
        QObservableList<int> list;

        bool handlerHasBeenCalled = false;
        list.afterAdd() += [&handlerHasBeenCalled, &list](int index, const int &value) {
            handlerHasBeenCalled = true;
            QCOMPARE(index, 0);
            QCOMPARE(value, 10);
            QCOMPARE(list.isEmpty(), false);
        };

        list.insert(0, 10);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void beforeClear_listHasOneElement_handlerIsCalledBeforeListIsCleared()
    {
        QObservableList<int> list;
        list.append(10);

        bool handlerHasBeenCalled = false;
        list.beforeClear() += [&handlerHasBeenCalled, &list]() {
            handlerHasBeenCalled = true;
            QCOMPARE(list.isEmpty(), false);
        };

        list.clear();

        QCOMPARE(list.isEmpty(), true);
        QCOMPARE(handlerHasBeenCalled, true);
    }

    void afterClear_listHasOneElement_handlerIsCalledAfterListIsCleared()
    {
        QObservableList<int> list;
        list.append(10);

        bool handlerHasBeenCalled = false;
        list.afterClear() += [&handlerHasBeenCalled, &list]() {
            handlerHasBeenCalled = true;
            QCOMPARE(list.isEmpty(), true);
        };

        list.clear();

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void beforeRemove_listHasOneElement_handlerIsCalledBeforeListIsDecreased()
    {
        QObservableList<int> list;
        list.append(10);

        bool handlerHasBeenCalled = false;
        list.beforeRemove() += [&handlerHasBeenCalled, &list](int index, const int &value) {
            handlerHasBeenCalled = true;
            QCOMPARE(value, 10);
            QCOMPARE(list.isEmpty(), false);
        };

        list.removeAt(0);

        QCOMPARE(list.isEmpty(), true);
        QCOMPARE(handlerHasBeenCalled, true);
    }

    void afterRemove_listHasOneElement_handlerIsCalledAfterListIsDecreased()
    {
        QObservableList<int> list;
        list.append(10);

        bool handlerHasBeenCalled = false;
        list.afterRemove() += [&handlerHasBeenCalled, &list](int index, const int &value) {
            handlerHasBeenCalled = true;
            QCOMPARE(value, 10);
            QCOMPARE(list.isEmpty(), true);
        };

        list.removeAt(0);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void beforeAddBatch_listIsEmpty_handlerIsCalledBeforeListIsIncreased()
    {
        QObservableList<int> list;

        bool handlerHasBeenCalled = false;
        list.beforeAddBatch() += [&handlerHasBeenCalled, &list](int index, const QList<int> &value) {
            handlerHasBeenCalled = true;
            QCOMPARE(value.at(0), 10);
            QCOMPARE(value.at(1), 15);
            QCOMPARE(list.isEmpty(), true);
        };

        list.append(QList<int>() << 10 << 15);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void afterAddBatch_listIsEmpty_handlerIsCalledAfterListIsIncreased()
    {
        QObservableList<int> list;

        bool handlerHasBeenCalled = false;
        list.afterAddBatch() += [&handlerHasBeenCalled, &list](int index, const QList<int> &value) {
            handlerHasBeenCalled = true;
            QCOMPARE(value.at(0), 10);
            QCOMPARE(value.at(1), 15);
            QCOMPARE(list.isEmpty(), false);
        };

        list.append(QList<int>() << 10 << 15);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void beforeAddBatchForStdVector_listIsEmpty_handlerIsCalledBeforeListIsIncreased()
    {
        QObservableList<int> list;

        bool handlerHasBeenCalled = false;
        list.beforeAddBatch() += [&handlerHasBeenCalled, &list](int index, const QList<int> &value) {
            handlerHasBeenCalled = true;
            QCOMPARE(value.at(0), 10);
            QCOMPARE(value.at(1), 15);
            QCOMPARE(list.isEmpty(), true);
        };

        std::vector<int> tmp;
        tmp.push_back(10);
        tmp.push_back(15);

        list.append(tmp);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void afterAddBatchForStdVector_listIsEmpty_handlerIsCalledAfterListIsIncreased()
    {
        QObservableList<int> list;

        bool handlerHasBeenCalled = false;
        list.afterAddBatch() += [&handlerHasBeenCalled, &list](int index, const QList<int> &value) {
            handlerHasBeenCalled = true;
            QCOMPARE(value.at(0), 10);
            QCOMPARE(value.at(1), 15);
            QCOMPARE(list.isEmpty(), false);
        };

        std::vector<int> tmp;
        tmp.push_back(10);
        tmp.push_back(15);

        list.append(tmp);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void afterChangeForAppend_listIsEmpty_handlerIsCalledAfterListHasChanged()
    {
        QObservableList<int> list;

        bool handlerHasBeenCalled = false;
        list.afterChange() += [&handlerHasBeenCalled]() {
            handlerHasBeenCalled = true;
        };

        list.append(10);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void afterChangeForAppendBatch_listIsEmpty_handlerIsCalledAfterListHasChanged()
    {
        QObservableList<int> list;

        bool handlerHasBeenCalled = false;
        list.afterChange() += [&handlerHasBeenCalled]() {
            handlerHasBeenCalled = true;
        };

        list.append(QList<int>() << 10 << 15);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void afterChangeForAppendBatchStdVector_listIsEmpty_handlerIsCalledAfterListHasChanged()
    {
        QObservableList<int> list;

        bool handlerHasBeenCalled = false;
        list.afterChange() += [&handlerHasBeenCalled]() {
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
        QObservableList<int> list;

        bool handlerHasBeenCalled = false;
        list.afterChange() += [&handlerHasBeenCalled]() {
            handlerHasBeenCalled = true;
        };

        list.insert(0, 10);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void afterChangeForRemove_listHasOneElement_handlerIsCalledAfterListHasChanged()
    {
        QObservableList<int> list;
        list.append(10);

        bool handlerHasBeenCalled = false;
        list.afterChange() += [&handlerHasBeenCalled]() {
            handlerHasBeenCalled = true;
        };

        list.removeAt(0);

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void afterChangeForClear_listHasOneElement_handlerIsCalledAfterListHasChanged()
    {
        QObservableList<int> list;
        list.append(10);

        bool handlerHasBeenCalled = false;
        list.afterChange() += [&handlerHasBeenCalled]() {
            handlerHasBeenCalled = true;
        };

        list.clear();

        QCOMPARE(handlerHasBeenCalled, true);
    }

    void afterChangeForClear_listHasNoElement_handlerIsNotCalledAfterListHasChanged()
    {
        QObservableList<int> list;

        bool handlerHasBeenCalled = false;
        list.afterChange() += [&handlerHasBeenCalled]() {
            handlerHasBeenCalled = true;
        };

        list.clear();

        QCOMPARE(handlerHasBeenCalled, false);
    }
};

QTEST_APPLESS_MAIN(QObservableListTest)

#include "QObservableListTest.moc"
