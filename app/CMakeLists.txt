set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(src)

set(source-views
    src/views/mainwindow.h
    src/views/todosview.h
    src/views/todoview.h
    src/views/personsview.h
    src/views/personview.h
    src/views/mainwindow.cpp
    src/views/todosview.cpp
    src/views/todoview.cpp
    src/views/personsview.cpp
    src/views/personview.cpp)
source_group("views" FILES ${source-views})

set(source-data
    src/data/todo.h
    src/data/person.h
    src/data/todo.cpp
    src/data/person.cpp)
source_group("data" FILES ${source-data})

set(source-viewmodels
    src/viewmodels/todosviewmodel.h
    src/viewmodels/todoviewmodel.h
    src/viewmodels/personsviewmodel.h
    src/viewmodels/personviewmodel.h
    src/viewmodels/todosviewmodel.cpp
    src/viewmodels/todoviewmodel.cpp
    src/viewmodels/personsviewmodel.cpp
    src/viewmodels/personviewmodel.cpp)
source_group("viewmodels" FILES ${source-viewmodels})

set(source-dal
    src/dal/todorepository.h
    src/dal/todomapping.h
    src/dal/sqlitedatetimeconverter.h
    src/dal/sqlitebooleanconverter.h
    src/dal/personmapping.h
    src/dal/todoworkersquery.h
    src/dal/personrepository.h
    src/dal/todoworkerrepository.h
    src/dal/todoworkers.h
    src/dal/todorepository.cpp
    src/dal/todomapping.cpp
    src/dal/sqlitedatetimeconverter.cpp
    src/dal/sqlitebooleanconverter.cpp
    src/dal/personmapping.cpp
    src/dal/todoworkersquery.cpp
    src/dal/personrepository.cpp
    src/dal/todoworkerrepository.cpp
    src/dal/todoworkers.cpp)
source_group("dal" FILES ${source-dal})

set(source-itemmodels
    src/itemmodels/personlistmodel.h
    src/itemmodels/personlistmodel.cpp)
source_group("itemmodels" FILES ${source-itemmodels})

set(source-mvvm
    src/mvvm/lineeditbinding.h
    src/mvvm/lineeditbinding.cpp
    src/mvvm/plaintexteditbinding.h
    src/mvvm/plaintexteditbinding.cpp
    src/mvvm/checkboxbinding.h
    src/mvvm/checkboxbinding.cpp
    src/mvvm/datetimeeditbinding.h
    src/mvvm/datetimeeditbinding.cpp)
source_group("mvvm" FILES ${source-mvvm})

set(src 
    ${source-views} 
    ${source-viewmodels} 
    ${source-data} 
    ${source-dal} 
    ${source-itemmodels} 
    ${source-mvvm} 
    src/main.cpp)
source_group("" FILES src/main.cpp)

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
set_property(TARGET QMetaOrm PROPERTY FOLDER "ext")

set(EXTRA_LIBS ${EXTRA_LIBS} QMetaOrm)

add_executable(todo-manager-sample ${src} res/scripts.qrc)
target_link_libraries(todo-manager-sample ${EXTRA_LIBS})