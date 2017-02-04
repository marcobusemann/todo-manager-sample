#pragma once

#include <QWidget>

class QAbstractItemModel;
class TodosViewModel;

namespace Ui {
    class TodosView;
}

class TodosView : public QWidget
{
    Q_OBJECT

public:
    explicit TodosView(TodosViewModel *viewModel, QWidget *parent = 0);
    ~TodosView();

    virtual void showEvent(QShowEvent *event) override;

private:
    TodosViewModel *m_viewModel;
    Ui::TodosView *ui;

    bool m_firstShow;

private slots:
    void updateItemModel(QAbstractItemModel *model);
    void updateSelection();
};
