#pragma once

#include <QAbstractTableModel>
#include <QSharedPointer>

#include <dal/todorepository.h>
#include <dal/personrepository.h>
#include <data/todo.h>

class QSortFilterProxyModel;
class QAction;

class TodosViewModel : public QAbstractTableModel
{
    Q_OBJECT

    Q_PROPERTY(QString newTodoTitle READ getNewTodoTitle WRITE setNewTodoTitle NOTIFY newTodoTitleChanged)

public:
    enum Column {
        Title = 0,
        EndDate,
        Last
    };

    static QSharedPointer<TodosViewModel> factory(
        const TodoRepository::Ptr &todoRepository,
        const PersonRepository::Ptr &personRepository);

public:
    TodosViewModel(
        const TodoRepository::Ptr &todoRepository,
        const PersonRepository::Ptr &personRepository,
        QObject *parent = nullptr);
    void initialize();

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    QAction *getActionAdd() const;
    QAction *getActionEdit() const;
    QAction *getActionRemove() const;

    QString getNewTodoTitle() const { return m_newTodoTitle; }

public slots:
    void addTodo(const QString &title);
    void setNewTodoTitle(const QString &title);
    void updateSelection(const QModelIndexList &selectedIndexes);
    void setFilter(const QString &filter);

private:
    void setTodos(const QList<Todo::Ptr> &todos);

    TodoRepository::Ptr m_todoRepository;
    PersonRepository::Ptr m_personRepository;
    QSortFilterProxyModel *m_filterProxyModel;

    QList<QPersistentModelIndex> m_selectedTodos;
    QList<Todo::Ptr> m_todos;
    QString m_newTodoTitle;

    QAction *m_actionAdd;
    QAction *m_actionEdit;
    QAction *m_actionRemove;

private slots:
    void addByAction();
    void editByAction();
    void removeByAction();

signals:
    void itemModelChanged(QAbstractItemModel *model);
    void newTodoTitleChanged(const QString &newTodoTitle);
};
