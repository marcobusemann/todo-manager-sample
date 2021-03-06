#pragma once

#include <QSharedPointer>
#include <QScopedPointer>
#include <QWidget>

class QAbstractProxyModel;
class PersonsViewModel;

namespace Ui {
    class PersonsView;
}

class PersonsView : public QWidget
{
    Q_OBJECT

public:
    PersonsView(
        const QSharedPointer<PersonsViewModel> &viewModel,
        QWidget *parent = 0);

    virtual void showEvent(QShowEvent *event) override;

private:
    QSharedPointer<PersonsViewModel> m_viewModel;
    QScopedPointer<Ui::PersonsView> m_ui;
    QAbstractProxyModel *m_model;

    bool m_firstShow;

private slots:
    void updateSelection();
};
