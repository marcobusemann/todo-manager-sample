#include "person.h"

Person::Person()
    : m_id(0)
{

}

Person::Person(const Person &rhs)
    : m_id(rhs.m_id)
    , m_name(rhs.m_name)
    , m_surname(rhs.m_surname)
{

}

Person &Person::operator=(const Person &rhs)
{
    copy(rhs);
}

void Person::copy(const QSharedPointer<Person> &rhs)
{
    copy(*rhs);
}

void Person::copy(const Person &rhs)
{
    m_id = rhs.m_id;
    m_name = rhs.m_name;
    m_surname = rhs.m_surname;
}

void Person::setId(int id)
{
    if (id == m_id) return;
    m_id = id;
    emit idChanged(id);
}

int Person::getId() const
{
    return m_id;
}

void Person::setName(const QString &name)
{
    if (name == m_name) return;
    m_name = name;
    emit nameChanged(name);
}

const QString &Person::getName() const
{
    return m_name;
}

void Person::setSurname(const QString &surname)
{
    if (surname == m_surname) return;
    m_surname = surname;
    emit surnameChanged(surname);
}

const QString &Person::getSurname() const
{
    return m_surname;
}

QString Person::getFullName() const
{
    return QString("%2, %1").arg(m_surname, m_name);
}
