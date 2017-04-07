#pragma once

#include <QWidget>
#include <QMetaProperty>

namespace Ui {
    class PersonView;
}

class PersonViewModel;

class PersonView : public QWidget
{
    Q_OBJECT

public:
    PersonView(PersonViewModel *personViewModel, QWidget *parent = 0);
    ~PersonView();

private:
    Ui::PersonView *ui;
    PersonViewModel *m_viewModel;
};
