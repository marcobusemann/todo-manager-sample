#pragma once

#include <QObject>

#include <data/person.h>

class PersonViewModel : public QObject
{
    Q_OBJECT

public:
    PersonViewModel(QObject *parent = 0);

    void setPerson(const Person::Ptr &person);

    /*
        Build a new person object based on the
        data entered in the dialog.
    */
    Person::Ptr buildPerson() const;

    /*
        Use this model for view bindings.
    */
    Person::Ptr getModel() const;

private:
    Person::Ptr m_person;
};