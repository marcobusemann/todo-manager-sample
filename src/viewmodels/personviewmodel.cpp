#include "personviewmodel.h"

PersonViewModel::PersonViewModel(QObject *parent)
    : QObject(parent)
    , m_person(new Person())
{

}

void PersonViewModel::setPerson(const Person::Ptr &person)
{
    m_person->setId(person->getId());
    setName(person->getName());
    setSurname(person->getSurname());
}

Person::Ptr PersonViewModel::buildPerson() const
{
    Person::Ptr result(new Person());
    result->setId(m_person->getId());
    result->setName(m_person->getName());
    result->setSurname(m_person->getSurname());
    return result;
}

void PersonViewModel::setName(const QString &name)
{
    if (m_person->getName() == name) return;
    m_person->setName(name);
    emit nameChanged(m_person->getName());
}

void PersonViewModel::setSurname(const QString &surname)
{
    if (m_person->getSurname() == surname) return;
    m_person->setSurname(surname);
    emit surnameChanged(m_person->getSurname());
}

const QString &PersonViewModel::getName() const
{
    return m_person->getName();
}

const QString &PersonViewModel::getSurname() const
{
    return m_person->getSurname();
}
