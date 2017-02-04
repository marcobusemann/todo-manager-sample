QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = todo-manager-sample
TEMPLATE = app

INCLUDEPATH += src

RESOURCES += \
    resources/scripts.qrc

SOURCES += \
   src/main.cpp \
   src/views/mainwindow.cpp \
   src/data/todo.cpp \
   src/views/todosview.cpp \
   src/viewmodels/todosviewmodel.cpp \
   src/dal/todorepository.cpp \
   src/dal/todomapping.cpp \
   src/dal/sqlitedatetimeconverter.cpp \
   src/dal/sqlitebooleanconverter.cpp \
   src/views/todoview.cpp \
   src/viewmodels/todoviewmodel.cpp \
    src/data/person.cpp \
    src/dal/personmapping.cpp \
    src/dal/todoworkersquery.cpp \
    src/dal/personrepository.cpp \
    src/views/personsview.cpp \
    src/views/personview.cpp \
    src/viewmodels/personsviewmodel.cpp \
    src/viewmodels/personviewmodel.cpp \
   src/itemmodels/personlistmodel.cpp \
    src/dal/todoworkerrepository.cpp \
    src/dal/todoworkers.cpp

HEADERS += \
   src/views/mainwindow.h \
   src/data/todo.h \
   src/views/todosview.h \
   src/viewmodels/todosviewmodel.h \
   src/dal/todorepository.h \
   src/dal/todomapping.h \
   src/dal/sqlitedatetimeconverter.h \
   src/dal/sqlitebooleanconverter.h \
   src/views/todoview.h \
   src/viewmodels/todoviewmodel.h \
    src/data/person.h \
    src/dal/personmapping.h \
    src/dal/todoworkersquery.h \
    src/dal/personrepository.h \
    src/views/personsview.h \
    src/views/personview.h \
    src/viewmodels/personsviewmodel.h \
    src/viewmodels/personviewmodel.h \
   src/itemmodels/personlistmodel.h \
    src/dal/todoworkerrepository.h \
    src/dal/todoworkers.h

FORMS   += \
   src/views/mainwindow.ui \
   src/views/todosview.ui \
    src/views/todoview.ui \
    src/views/personsview.ui \
    src/views/personview.ui

# RxCPP
INCLUDEPATH += \
   ext/RxCpp/Ix/CPP/src \
   ext/RxCpp/Rx/v2/src \

# QMetaOrm
include(ext/QMetaOrm/QMetaOrm.pri)
