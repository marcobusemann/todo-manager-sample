#pragma once

#include <QSharedPointer>
#include <QObject>
#include <QList>

class Person : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString surname READ getSurname WRITE setSurname NOTIFY surnameChanged)

public:
    typedef QSharedPointer<Person> Ptr;

public:
    Person();
    Person(const Person &rhs);
    Person &operator=(const Person &rhs);
    bool operator==(const Person &rhs);

    void setId(int id);
    int getId() const;

    const QString &getName() const;
    const QString &getSurname() const;

    QString getFullName() const;

    void copy(const Person &rhs);
    void copy(const QSharedPointer<Person> &rhs);

public slots:
    void setName(const QString &name);
    void setSurname(const QString &surname);

private:
    int m_id;
    QString m_name;
    QString m_surname;

signals:
    void idChanged(int id);
    void nameChanged(const QString &name);
    void surnameChanged(const QString &surname);
};

Q_DECLARE_METATYPE(Person::Ptr)
Q_DECLARE_METATYPE(QList<Person::Ptr>)
