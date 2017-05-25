#pragma once

#include <QAbstractTableModel>
#include <QSharedPointer>

#include <dal/todorepository.h>
#include <dal/personrepository.h>
#include <data/todo.h>

#include <moderngrids\qmgobservablelist.h>

class QAction;

class TodosViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString newTodoTitle READ getNewTodoTitle WRITE setNewTodoTitle NOTIFY newTodoTitleChanged)

public:
    static QSharedPointer<TodosViewModel> factory(
        const TodoRepository::Ptr &todoRepository,
        const PersonRepository::Ptr &personRepository);

public:
    TodosViewModel(
        const TodoRepository::Ptr &todoRepository,
        const PersonRepository::Ptr &personRepository,
        QObject *parent = nullptr);
    void initialize();

    QAction *getActionAdd() const;
    QAction *getActionEdit() const;
    QAction *getActionRemove() const;

    QString getNewTodoTitle() const { return m_newTodoTitle; }

	QmgObservableList<Todo::Ptr> &getTodos() { return m_todos; }

public slots:
    void addTodo(const QString &title);
    void setNewTodoTitle(const QString &title);
    void updateSelection(const QModelIndexList &selectedIndexes);

private:
    TodoRepository::Ptr m_todoRepository;
    PersonRepository::Ptr m_personRepository;

    QList<QPersistentModelIndex> m_selectedTodos;
	QmgObservableList<Todo::Ptr> m_todos;
    QString m_newTodoTitle;

    QAction *m_actionAdd;
    QAction *m_actionEdit;
    QAction *m_actionRemove;

private slots:
    void addByAction();
    void editByAction();
    void removeByAction();

signals:
    void newTodoTitleChanged(const QString &newTodoTitle);
};
