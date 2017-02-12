#include "personview.h"
#include "ui_personview.h"

#include <viewmodels/personviewmodel.h>

PersonView::PersonView(PersonViewModel *personViewModel, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonView),
    m_viewModel(personViewModel)
{
    ui->setupUi(this);

    connect(ui->editName, &QLineEdit::textChanged, m_viewModel, &PersonViewModel::setName);
    connect(m_viewModel, &PersonViewModel::nameChanged, this, &PersonView::updateName);

    connect(ui->editSurname, &QLineEdit::textChanged, m_viewModel, &PersonViewModel::setSurname);
    connect(m_viewModel, &PersonViewModel::surnameChanged, this, &PersonView::updateSurname);
}

PersonView::~PersonView()
{
    delete ui;
}

void PersonView::updateName(const QString &name)
{
    if (ui->editName->text() == name) return;
    ui->editName->setText(name);
}

void PersonView::updateSurname(const QString &surname)
{
    if (ui->editSurname->text() == surname) return;
    ui->editSurname->setText(surname);
}
