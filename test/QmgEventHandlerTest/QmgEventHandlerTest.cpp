#include <QtTest>

#include <moderngrids\qmgeventhandler.h>

class QmgEventHandlerTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void constructor_byDefaultNoHandlers()
    {
        QmgEventHandler<void()> eventHandler;

        QCOMPARE(eventHandler.isEmpty(), true);
    }

    void attach_empty_containsOneHandler()
    {
        QmgEventHandler<void()> eventHandler;

        auto scope = eventHandler.attach([]() {});

        QCOMPARE(eventHandler.isEmpty(), false);
    }

    void attach_empty_containsNoHandlerCauseScopeNotSaved()
    {
        QmgEventHandler<void()> eventHandler;

        eventHandler.attach([]() {});

        QCOMPARE(eventHandler.isEmpty(), true);
    }

    void attachUnsafe_empty_containsOneHandler()
    {
        QmgEventHandler<void()> eventHandler;

        eventHandler.attachUnsafe([]() {});

        QCOMPARE(eventHandler.isEmpty(), false);
    }

    void detach_containsOneHandler_empty()
    {
        QmgEventHandler<void()> eventHandler;
        auto scope = eventHandler.attach([]() {});
        QCOMPARE(eventHandler.isEmpty(), false);

        eventHandler.detach(scope);

        QCOMPARE(eventHandler.isEmpty(), true);
    }

    void detachOfScope_containsOneHandler_empty()
    {
        QmgEventHandler<void()> eventHandler;
        auto scope = eventHandler.attach([]() {});
        QCOMPARE(eventHandler.isEmpty(), false);

        scope->detach();

        QCOMPARE(eventHandler.isEmpty(), true);
    }

    void emptyOperator_containsNoHandler_nothingTodo()
    {
        QmgEventHandler<void()> eventHandler;

        eventHandler();
    }

    void emptyOperator_containsOneHandler_executeThatHandler()
    {
        QmgEventHandler<void()> eventHandler;

        bool hasBeenExecuted = false;
        auto scope = eventHandler.attach([&hasBeenExecuted]() {
            hasBeenExecuted = true;
        });

        eventHandler();
        QCOMPARE(hasBeenExecuted, true);
    }

    void oneParamOperator_containsOneHandler_executeThatHandler()
    {
        QmgEventHandler<void(int)> eventHandler;

        int value = 0;
        auto scope = eventHandler.attach([&value](int a) {
            value = a;
        });

        eventHandler(10);
        QCOMPARE(value, 10);
    }

    void twoParamOperator_containsOneHandler_executeThatHandler()
    {
        QmgEventHandler<void(int, int)> eventHandler;

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
        QmgScopePtr scope;
        
        {
            QmgEventHandler<void()> eventHandler;
            scope = eventHandler.attach([]() {});
        }
    }
};

QTEST_APPLESS_MAIN(QmgEventHandlerTest)

#include "QmgEventHandlerTest.moc"
