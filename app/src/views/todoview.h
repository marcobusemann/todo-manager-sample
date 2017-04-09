#pragma once

#include <QScopedPointer>
#include <QSharedPointer>
#include <QWidget>

class TodoViewModel;

namespace Ui {
    class TodoView;
}

class TodoView : public QWidget
{
    Q_OBJECT

public:
    TodoView(
        const QSharedPointer<TodoViewModel> &viewModel,
        QWidget *parent = 0);

    virtual void showEvent(QShowEvent *event) override;

private:
    QSharedPointer<TodoViewModel> m_viewModel;
    QScopedPointer<Ui::TodoView> m_ui;

    bool m_firstShow;
};
