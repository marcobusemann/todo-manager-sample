#include "person.h"

Person::Person()
    : m_id(0)
{

}

void Person::setId(int id)
{
    m_id = id;
}

int Person::getId() const
{
    return m_id;
}

void Person::setName(const QString &name)
{
    m_name = name;
}

const QString &Person::getName() const
{
    return m_name;
}

void Person::setSurname(const QString &surname)
{
    m_surname = surname;
}

const QString &Person::getSurname() const
{
    return m_surname;
}

QString Person::getFullName() const
{
    return QString("%2, %1").arg(m_surname, m_name);
}
