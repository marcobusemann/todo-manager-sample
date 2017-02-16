set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(src)

set(header
    src/views/mainwindow.h
    src/data/todo.h
    src/views/todosview.h
    src/viewmodels/todosviewmodel.h
    src/dal/todorepository.h
    src/dal/todomapping.h
    src/dal/sqlitedatetimeconverter.h
    src/dal/sqlitebooleanconverter.h
    src/views/todoview.h
    src/viewmodels/todoviewmodel.h
    src/data/person.h
    src/dal/personmapping.h
    src/dal/todoworkersquery.h
    src/dal/personrepository.h
    src/views/personsview.h
    src/views/personview.h
    src/viewmodels/personsviewmodel.h
    src/viewmodels/personviewmodel.h
    src/itemmodels/personlistmodel.h
    src/dal/todoworkerrepository.h
    src/dal/todoworkers.h
    src/mvvm/lineeditbinding.h)

set(src
    src/main.cpp
    src/views/mainwindow.cpp
    src/data/todo.cpp
    src/views/todosview.cpp
    src/viewmodels/todosviewmodel.cpp
    src/dal/todorepository.cpp
    src/dal/todomapping.cpp
    src/dal/sqlitedatetimeconverter.cpp
    src/dal/sqlitebooleanconverter.cpp
    src/views/todoview.cpp
    src/viewmodels/todoviewmodel.cpp
    src/data/person.cpp
    src/dal/personmapping.cpp
    src/dal/todoworkersquery.cpp
    src/dal/personrepository.cpp
    src/views/personsview.cpp
    src/views/personview.cpp
    src/viewmodels/personsviewmodel.cpp
    src/viewmodels/personviewmodel.cpp
    src/itemmodels/personlistmodel.cpp
    src/dal/todoworkerrepository.cpp
    src/dal/todoworkers.cpp
    src/mvvm/lineeditbinding.cpp)

# Bind to qt
find_package(Qt5Core)
find_package(Qt5Sql)
find_package(Qt5Widgets)

if(Qt5Core_FOUND AND Qt5Sql_FOUND AND Qt5Widgets_FOUND)
    set (EXTRA_LIBS ${EXTRA_LIBS} Qt5::Core Qt5::Sql Qt5::Widgets)    
else()
    find_package(Qt4 REQUIRED)
    set (EXTRA_LIBS ${EXTRA_LIBS} Qt4::Core Qt4::Sql Qt4::Widgets)
endif()

add_subdirectory(ext/QMetaOrm)

set(EXTRA_LIBS ${EXTRA_LIBS} QMetaOrm)

add_executable(todo-manager-sample ${header} ${src} res/scripts.qrc)
target_link_libraries(todo-manager-sample ${EXTRA_LIBS})