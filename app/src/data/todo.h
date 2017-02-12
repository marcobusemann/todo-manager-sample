#pragma once

#include <QSharedPointer>
#include <QDateTime>
#include <QObject>
#include <QString>

#include <data/person.h>

class Todo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId WRITE setId)
    Q_PROPERTY(QString title READ getTitle WRITE setTitle)
    Q_PROPERTY(QString description READ getDescription WRITE setDescription)
    Q_PROPERTY(QDateTime endDate READ getEndDate WRITE setEndDate)
    Q_PROPERTY(bool completed READ isCompleted WRITE setCompleted)
    Q_PROPERTY(Person::Ptr owner READ getOwner WRITE setOwner)

public:
    typedef QSharedPointer<Todo> Ptr;

public:
    Todo();

    int getId() const;
    const QString &getTitle() const;
    const QString &getDescription() const;
    const QDateTime &getEndDate() const;
    bool isCompleted() const;
    const Person::Ptr &getOwner() const;
    const QList<Person::Ptr> &getWorkers() const;

    void setId(int aId);
    void setTitle(const QString &title);
    void setDescription(const QString &description);
    void setEndDate(const QDateTime &endDate);
    void setCompleted(bool isCompleted);
    void setOwner(const Person::Ptr &owner);
    void setWorkers(const QList<Person::Ptr> &workers);

private:
    int m_id;
    QString m_title;
    QString m_description;
    QDateTime m_endDate;
    bool m_isCompleted;
    Person::Ptr m_owner;
    QList<Person::Ptr> m_workers;
};

Q_DECLARE_METATYPE(Todo::Ptr)
