#ifndef PERSONSVIEW_H
#define PERSONSVIEW_H

#include <QWidget>

class QAbstractItemModel;
class PersonsViewModel;

namespace Ui {
class PersonsView;
}

class PersonsView : public QWidget
{
    Q_OBJECT

public:
    explicit PersonsView(PersonsViewModel *viewModel, QWidget *parent = 0);
    ~PersonsView();

    virtual void showEvent(QShowEvent *event) override;

private:
    PersonsViewModel *m_viewModel;
    Ui::PersonsView *ui;

    bool m_firstShow;

private slots:
    void updateItemModel(QAbstractItemModel *model);
    void updateSelection();
};

#endif // PERSONSVIEW_H
