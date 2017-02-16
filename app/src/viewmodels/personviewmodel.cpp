#include "personviewmodel.h"

PersonViewModel::PersonViewModel(QObject *parent)
    : QObject(parent)
    , m_person(new Person())
{

}

void PersonViewModel::setPerson(const Person::Ptr &person)
{
    m_person->copy(person);
}

Person::Ptr PersonViewModel::buildPerson() const
{
    Person::Ptr result(new Person());
    result->copy(m_person);
    return result;
}

Person::Ptr PersonViewModel::getModel() const
{
    return m_person;
}

