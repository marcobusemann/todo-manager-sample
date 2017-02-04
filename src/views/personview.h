#ifndef PERSONVIEW_H
#define PERSONVIEW_H

#include <QWidget>

namespace Ui {
class PersonView;
}

class PersonViewModel;

class PersonView : public QWidget
{
    Q_OBJECT

public:
    explicit PersonView(PersonViewModel *personViewModel, QWidget *parent = 0);
    ~PersonView();

private:
    Ui::PersonView *ui;
    PersonViewModel *m_viewModel;

private slots:
    void updateName(const QString &name);
    void updateSurname(const QString &surname);
};

#endif // PERSONVIEW_H
