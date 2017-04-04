#include <QtTest>

#include <moderngrids\qeventhandler.h>

class QEventHandlerTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void constructor_byDefaultNoHandlers()
    {
        QEventHandler<void()> eventHandler;

        QCOMPARE(eventHandler.isEmpty(), true);
    }

    void attach_empty_containsOneHandler()
    {
        QEventHandler<void()> eventHandler;

        auto scope = eventHandler.attach([]() {});

        QCOMPARE(eventHandler.isEmpty(), false);
    }

    void attach_empty_containsNoHandlerCauseScopeNotSaved()
    {
        QEventHandler<void()> eventHandler;

        eventHandler.attach([]() {});

        QCOMPARE(eventHandler.isEmpty(), true);
    }

    void attachUnsafe_empty_containsOneHandler()
    {
        QEventHandler<void()> eventHandler;

        eventHandler.attachUnsafe([]() {});

        QCOMPARE(eventHandler.isEmpty(), false);
    }

    void detach_containsOneHandler_empty()
    {
        QEventHandler<void()> eventHandler;
        auto scope = eventHandler.attach([]() {});
        QCOMPARE(eventHandler.isEmpty(), false);

        eventHandler.detach(scope);

        QCOMPARE(eventHandler.isEmpty(), true);
    }

    void detachOfScope_containsOneHandler_empty()
    {
        QEventHandler<void()> eventHandler;
        auto scope = eventHandler.attach([]() {});
        QCOMPARE(eventHandler.isEmpty(), false);

        scope->detach();

        QCOMPARE(eventHandler.isEmpty(), true);
    }

    void emptyOperator_containsNoHandler_nothingTodo()
    {
        QEventHandler<void()> eventHandler;

        eventHandler();
    }

    void emptyOperator_containsOneHandler_executeThatHandler()
    {
        QEventHandler<void()> eventHandler;

        bool hasBeenExecuted = false;
        auto scope = eventHandler.attach([&hasBeenExecuted]() {
            hasBeenExecuted = true;
        });

        eventHandler();
        QCOMPARE(hasBeenExecuted, true);
    }

    void oneParamOperator_containsOneHandler_executeThatHandler()
    {
        QEventHandler<void(int)> eventHandler;

        int value = 0;
        auto scope = eventHandler.attach([&value](int a) {
            value = a;
        });

        eventHandler(10);
        QCOMPARE(value, 10);
    }

    void twoParamOperator_containsOneHandler_executeThatHandler()
    {
        QEventHandler<void(int, int)> eventHandler;

        int value1 = 0, value2 = 0;
        auto scope = eventHandler.attach([&value1, &value2](int a, int b) {
            value1 = a;
            value2 = b;
        });

        eventHandler(10, 15);
        QCOMPARE(value1, 10);
        QCOMPARE(value2, 15);
    }

    void lifetimeOfScopeOutlifesHandler_itsOkButShouldNotCrash()
    {
        QScopePtr scope;
        
        {
            QEventHandler<void()> eventHandler;
            scope = eventHandler.attach([]() {});
        }
    }
};

QTEST_APPLESS_MAIN(QEventHandlerTest)

#include "QEventHandlerTest.moc"
