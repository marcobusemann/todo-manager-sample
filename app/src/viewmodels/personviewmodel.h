#ifndef PERSONVIEWMODEL_H
#define PERSONVIEWMODEL_H

#include <QObject>

#include <data/person.h>

class PersonViewModel : public QObject
{
    Q_OBJECT

public:
    explicit PersonViewModel(QObject *parent = 0);
    void setPerson(const Person::Ptr &person);
    Person::Ptr buildPerson() const;

    void setName(const QString &name);
    void setSurname(const QString &surname);

    const QString &getName() const;
    const QString &getSurname() const;

private:
    Person::Ptr m_person;

signals:
    void nameChanged(const QString &name);
    void surnameChanged(const QString &surname);
};

#endif // PERSONVIEWMODEL_H
