#pragma once

#include <QWidget>

#include <data/person.h>

class QAbstractItemModel;
class TodoViewModel;

namespace Ui {
    class TodoView;
}

class TodoView : public QWidget
{
    Q_OBJECT

public:
    explicit TodoView(TodoViewModel *viewModel, QWidget *parent = 0);
    ~TodoView();

    virtual void showEvent(QShowEvent *event) override;

private:
    TodoViewModel *m_viewModel;
    Ui::TodoView *ui;

    bool m_firstShow;

private slots:
    void updateAvailableOwners(const QList<Person::Ptr> &persons);
    void updateAvailableWorkers(const QList<Person::Ptr> &persons);
    void updateViewModelCurrentNewWorker();
    void updateOwner(const Person::Ptr &person);
    void updateViewModelOwner();
    void setWorkers(const QList<Person::Ptr> &persons);
};
