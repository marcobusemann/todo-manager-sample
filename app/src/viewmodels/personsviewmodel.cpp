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
    : QAbstractTableModel(parent)
    , m_personRepository(personRepository)
    , m_filterProxyModel(new QSortFilterProxyModel(this))
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

    m_filterProxyModel->setSourceModel(this);
    m_filterProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_filterProxyModel->setFilterKeyColumn(-1);
}

void PersonsViewModel::initialize()
{
    setPersons(m_personRepository->getAll());
    emit itemModelChanged(m_filterProxyModel);
}

int PersonsViewModel::rowCount(const QModelIndex &) const
{
    return m_persons.size();
}

int PersonsViewModel::columnCount(const QModelIndex &) const
{
    return Column::Last;
}

QVariant PersonsViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto &item = m_persons[index.row()];

    QVariant result;

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case Column::Name:
            result = item->getName();
            break;
        case Column::Surname:
            result = item->getSurname();
            break;
        }
    }

    return result;
}

QVariant PersonsViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();

    QVariant result;
    switch (section) {
    case Column::Name:
        result = tr("Name");
        break;
    case Column::Surname:
        result = tr("Surname");
        break;
    }
    return result;
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

    beginInsertRows(QModelIndex(), m_persons.size(), m_persons.size());
    m_persons.append(newPerson);
    endInsertRows();
}

void PersonsViewModel::setNewPersonNames(const QString &names)
{
    if (m_newPersonNames == names) return;
    m_newPersonNames = names;
    emit newPersonNamesChanged(m_newPersonNames);
}

void PersonsViewModel::updateSelection(const QModelIndexList &selectedIndexes)
{
    m_selectedPersons.clear();
    for (auto index : selectedIndexes) {
        m_selectedPersons.append(QPersistentModelIndex(m_filterProxyModel->mapToSource(index)));
    }

    m_actionEdit->setEnabled(m_selectedPersons.size() == 1);
    m_actionRemove->setEnabled(m_selectedPersons.size() > 0);
}

void PersonsViewModel::setFilter(const QString &filter)
{
    m_filterProxyModel->setFilterWildcard(filter);
}

void PersonsViewModel::setPersons(const QList<Person::Ptr> &persons)
{
    beginResetModel();
    m_persons = persons;
    endResetModel();
}

void PersonsViewModel::addByAction()
{
    if (!m_newPersonNames.isEmpty())
        addPerson(m_newPersonNames);
    setNewPersonNames(QString());
}

void PersonsViewModel::editByAction()
{
    auto index = m_selectedPersons[0];
    auto &item = m_persons[index.row()];

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
        m_persons[index.row()] = person;
        m_personRepository->save(person);
        emit dataChanged(index.sibling(index.row(), 0), index.sibling(index.row(), Column::Last - 1));
    }
}

void PersonsViewModel::removeByAction()
{
    if (m_selectedPersons.size() <= 0)
        return;

    auto personIndexesToRemove = m_selectedPersons;
    updateSelection(QModelIndexList());

    for (auto personIndex : personIndexesToRemove) {
        auto row = personIndex.row();
        beginRemoveRows(QModelIndex(), row, row);
        m_personRepository->remove(m_persons[row]);
        m_persons.removeAt(row);
        endRemoveRows();
    }
}
