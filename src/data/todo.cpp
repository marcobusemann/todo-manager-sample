#include "todo.h"

Todo::Todo()
    : m_id(0)
    , m_endDate(QDateTime::currentDateTime())
{}

int Todo::getId() const { return m_id; }
const QString &Todo::getTitle() const { return m_title; }
const QString &Todo::getDescription() const { return m_description; }
const QDateTime &Todo::getEndDate() const { return m_endDate; }
bool Todo::isCompleted() const { return m_isCompleted; }

const Person::Ptr &Todo::getOwner() const
{
    return m_owner;
}

const QList<Person::Ptr> &Todo::getWorkers() const
{
    return m_workers;
}

void Todo::setId(int aId) {
    m_id = aId;
}

void Todo::setTitle(const QString &title) {
    m_title = title;
}

void Todo::setDescription(const QString &description) {
    m_description = description;
}

void Todo::setEndDate(const QDateTime &endDate) {
    m_endDate = endDate;
}

void Todo::setCompleted(bool isCompleted) {
    m_isCompleted = isCompleted;
}

void Todo::setOwner(const Person::Ptr &owner)
{
    m_owner = owner;
}

void Todo::setWorkers(const QList<Person::Ptr> &workers)
{
    m_workers = workers;
}
