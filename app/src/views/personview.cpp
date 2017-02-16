#include "personview.h"
#include "ui_personview.h"

#include <viewmodels/personviewmodel.h>
#include <mvvm/lineeditbinding.h>

PersonView::PersonView(PersonViewModel *personViewModel, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonView),
    m_viewModel(personViewModel)
{
    ui->setupUi(this);

    LineEditBinding::factory(ui->editName, m_viewModel->getModel(), "name");
    LineEditBinding::factory(ui->editSurname, m_viewModel->getModel(), "surname");
}

PersonView::~PersonView()
{
    delete ui;
}