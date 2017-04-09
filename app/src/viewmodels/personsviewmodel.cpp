#include "personsviewmodel.h"

#include <QSortFilterProxyModel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QAction>
#include <QDialog>

#include <views/personview.h>
#include <viewmodels/personviewmodel.h>

QSharedPointer<PersonsViewModel> PersonsViewModel::factory(const PersonRepository::Ptr &personRepository)
{
    return QSharedPointer<PersonsViewModel>(new PersonsViewModel(personRepository));
}

PersonsViewModel::PersonsViewModel(const PersonRepository::Ptr &personRepository, QObject *parent)
    : QObject(parent)
    , m_personRepository(personRepository)
    , m_persons(QObservableList<Person::Ptr>::empty())
    , m_actionAdd(new QAction(this))
    , m_actionEdit(new QAction(this))
    , m_actionRemove(new QAction(this))
{
    connect(m_actionAdd, &QAction::triggered, this, &PersonsViewModel::addByAction);
    connect(m_actionEdit, &QAction::triggered, this, &PersonsViewModel::editByAction);
    connect(m_actionRemove, &QAction::triggered, this, &PersonsViewModel::removeByAction);

    m_actionAdd->setText(tr("Add person"));
    m_actionEdit->setText(tr("Edit person"));
    m_actionRemove->setText(tr("Remove person"));

    m_actionEdit->setEnabled(false);
    m_actionRemove->setEnabled(false);
}

void PersonsViewModel::initialize()
{
    m_persons = QObservableList<Person::Ptr>::fromList(m_personRepository->getAll());
}

QObservableList<Person::Ptr> &PersonsViewModel::getPersons()
{
    return m_persons;
}

QAction *PersonsViewModel::getActionAdd() const
{
    return m_actionAdd;
}

QAction *PersonsViewModel::getActionEdit() const
{
    return m_actionEdit;
}

QAction *PersonsViewModel::getActionRemove() const
{
    return m_actionRemove;
}

void PersonsViewModel::addPerson(const QString &names)
{
    if (!names.contains(",")) return;
    Person::Ptr newPerson(new Person());

    QStringList nameList = names.split(",");
    newPerson->setSurname(nameList[0].trimmed());
    newPerson->setName(nameList[1].trimmed());

    m_personRepository->save(newPerson);

    m_persons.append(newPerson);
}

void PersonsViewModel::setNewPersonName(const QString &name)
{
    if (m_newPersonName == name) return;
    m_newPersonName = name;
    emit newPersonNameChanged(m_newPersonName);
}

const QString &PersonsViewModel::getNewPersonName() const
{
    return m_newPersonName;
}

void PersonsViewModel::updateSelection(const QList<QPersistentModelIndex> &selectedItems)
{
    m_selectedPersons = selectedItems;
    m_actionEdit->setEnabled(m_selectedPersons.size() == 1);
    m_actionRemove->setEnabled(m_selectedPersons.size() > 0);
}

void PersonsViewModel::addByAction()
{
    if (!m_newPersonName.isEmpty())
        addPerson(m_newPersonName);
    setNewPersonName(QString());
}

void PersonsViewModel::editByAction()
{
    auto index = m_selectedPersons[0];
    auto &item = m_persons.at(index.row());

    QDialog dialog;

    auto personViewModel = PersonViewModel::factory();
    auto personView = new PersonView(personViewModel);
    personViewModel->setPerson(item);

    auto *buttonSave = new QPushButton();
    buttonSave->setText(tr("Save"));
    connect(buttonSave, &QPushButton::clicked, &dialog, &QDialog::accept);

    auto *buttonCancel = new QPushButton();
    buttonCancel->setText(tr("Cancel"));
    connect(buttonCancel, &QPushButton::clicked, &dialog, &QDialog::reject);

    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addItem(new QSpacerItem(-1, -1, QSizePolicy::Expanding));
    buttonLayout->addWidget(buttonSave);
    buttonLayout->addWidget(buttonCancel);

    auto *dialogLayout = new QVBoxLayout();
    dialogLayout->addWidget(personView);
    dialogLayout->addLayout(buttonLayout);
    dialogLayout->setMargin(0);

    dialog.setLayout(dialogLayout);
    auto execCode = dialog.exec();

    if (execCode == QDialog::Accepted) {
        auto person = personViewModel->buildPerson();
        m_persons.update(index.row(), person);
        m_personRepository->save(person);
    }
}

void PersonsViewModel::removeByAction()
{
    if (m_selectedPersons.size() <= 0)
        return;

    auto personIndexesToRemove = m_selectedPersons;
    updateSelection(QList<QPersistentModelIndex>());

    for (auto personIndex : personIndexesToRemove) {
        auto row = personIndex.row();
        m_personRepository->remove(m_persons.at(row));
        m_persons.removeAt(row);
    }
}
