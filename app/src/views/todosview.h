#pragma once

#include <QSharedPointer>
#include <QScopedPointer>
#include <QWidget>

class QAbstractProxyModel;
class TodosViewModel;

namespace Ui {
    class TodosView;
}

class TodosView : public QWidget
{
    Q_OBJECT

public:
    TodosView(
        const QSharedPointer<TodosViewModel> &viewModel, 
        QWidget *parent = 0);

    virtual void showEvent(QShowEvent *event) override;

private:
    QSharedPointer<TodosViewModel> m_viewModel;
    QScopedPointer<Ui::TodosView> m_ui;
    QAbstractProxyModel *m_model;

    bool m_firstShow;

private slots:
    void updateSelection();
};
