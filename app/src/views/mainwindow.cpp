#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <viewmodels/todosviewmodel.h>
#include <views/todosview.h>
#include <dal/todorepository.h>

#include <viewmodels/personsviewmodel.h>
#include <views/personsview.h>
#include <dal/personrepository.h>

MainWindow::MainWindow(
    const QMetaOrm::SessionFactory::Ptr &sessionFactory, 
    QWidget *parent) 
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow())
{
    m_ui->setupUi(this);

    auto personRepository = PersonRepository::factory(sessionFactory);
    auto todoRepository = TodoRepository::factory(sessionFactory);

    auto todosViewModel = TodosViewModel::factory(todoRepository, personRepository);
    auto todosView = new TodosView(todosViewModel, this);
    m_ui->layoutTodos->addWidget(todosView);

    auto personsViewModel = PersonsViewModel::factory(personRepository);
    auto personsView = new PersonsView(personsViewModel, this);
    m_ui->layoutPersons->addWidget(personsView);
}

MainWindow::~MainWindow() = default;
