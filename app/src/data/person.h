#pragma once

#include <QSharedPointer>
#include <QObject>

class Person : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId WRITE setId)
    Q_PROPERTY(QString name READ getName WRITE setName)
    Q_PROPERTY(QString surname READ getSurname WRITE setSurname)

public:
    typedef QSharedPointer<Person> Ptr;

public:
    Person();

    void setId(int id);
    int getId() const;

    void setName(const QString &name);
    const QString &getName() const;

    void setSurname(const QString &surname);
    const QString &getSurname() const;

    QString getFullName() const;

private:
    int m_id;
    QString m_name;
    QString m_surname;
};

Q_DECLARE_METATYPE(Person::Ptr)
