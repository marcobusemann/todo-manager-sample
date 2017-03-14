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
};
