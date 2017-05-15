#pragma once

#include <QSharedPointer>
#include <QDateTime>
#include <QObject>
#include <QString>

#include <data/person.h>

class Todo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QDateTime endDate READ getEndDate WRITE setEndDate NOTIFY endDateChanged)
    Q_PROPERTY(bool completed READ isCompleted WRITE setCompleted NOTIFY completionChanged)
    Q_PROPERTY(Person::Ptr owner READ getOwner WRITE setOwner NOTIFY ownerChanged)
    Q_PROPERTY(QList<Person::Ptr> workers READ getWorkers WRITE setWorkers NOTIFY workersChanged)

public:
    typedef QSharedPointer<Todo> Ptr;

public:
    Todo();
    Todo(const Todo &rhs);
    Todo &operator=(const Todo &rhs);
    bool operator==(const Todo &rhs);

    int getId() const;
    const QString &getTitle() const;
    const QString &getDescription() const;
    const QDateTime &getEndDate() const;
    bool isCompleted() const;
    const Person::Ptr &getOwner() const;
    const QList<Person::Ptr> &getWorkers() const;

    void copy(const Todo &rhs);
    void copy(const QSharedPointer<Todo> &rhs);

public slots:
    void setId(int id);
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

signals:
    void idChanged(int id);
    void titleChanged(const QString &title);
    void descriptionChanged(const QString &description);
    void endDateChanged(const QDateTime &endDate);
    void completionChanged(bool isCompleted);
    void ownerChanged(const Person::Ptr &owner);
    void workersChanged(const QList<Person::Ptr> &workers);
};

Q_DECLARE_METATYPE(Todo::Ptr)
