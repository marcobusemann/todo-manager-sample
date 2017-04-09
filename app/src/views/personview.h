#pragma once

#include <QSharedPointer>
#include <QScopedPointer>
#include <QWidget>

class PersonViewModel;

namespace Ui {
    class PersonView;
}

class PersonView : public QWidget
{
    Q_OBJECT

public:
    PersonView(
        const QSharedPointer<PersonViewModel> &personViewModel, 
        QWidget *parent = 0);

private:
    QSharedPointer<PersonViewModel> m_viewModel;
    QScopedPointer<Ui::PersonView> m_ui;
};
