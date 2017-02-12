#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <viewmodels/todosviewmodel.h>
#include <views/todosview.h>
#include <dal/todorepository.h>

#include <viewmodels/personsviewmodel.h>
#include <views/personsview.h>
#include <dal/personrepository.h>

MainWindow::MainWindow(const QMetaOrm::SessionFactory::Ptr &sessionFactory, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto personRepository = PersonRepository::factory(sessionFactory);
    auto todoRepository = TodoRepository::factory(sessionFactory);

    auto todosViewModel = new TodosViewModel(todoRepository, personRepository, this);
    auto todosView = new TodosView(todosViewModel, this);
    ui->layoutTodos->addWidget(todosView);

    auto personsViewModel = new PersonsViewModel(personRepository, this);
    auto personsView = new PersonsView(personsViewModel, this);
    ui->layoutPersons->addWidget(personsView);
}

MainWindow::~MainWindow()
{
    delete ui;
}
