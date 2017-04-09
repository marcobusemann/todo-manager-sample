#include "personview.h"

#include "ui_personview.h"

#include <viewmodels/personviewmodel.h>
#include <mvvm/lineeditbinding.h>

PersonView::PersonView(
    const QSharedPointer<PersonViewModel> &personViewModel,
    QWidget *parent)
    : QWidget(parent)
    , m_viewModel(personViewModel)
    , m_ui(new Ui::PersonView())
{
    m_ui->setupUi(this);

    LineEditBinding::factory(m_ui->editName, m_viewModel->getModel(), "name");
    LineEditBinding::factory(m_ui->editSurname, m_viewModel->getModel(), "surname");
}
