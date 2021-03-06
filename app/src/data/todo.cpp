#include "todo.h"
#include <functional>

template <typename T>
void SetProperty(const T &b, T &a, std::function<void()> changeCallback) {
    if (b == a) return;
    a = b;
    changeCallback();
}

Todo::Todo()
    : m_id(0)
    , m_isCompleted(false)
    , m_endDate(QDateTime::currentDateTime())
{}

Todo::Todo(const Todo &rhs)
{
   copy(rhs);
}

Todo &Todo::operator=(const Todo &rhs)
{
   copy(rhs);
   return *this;
}

bool Todo::operator==(const Todo &rhs)
{
   return m_id == rhs.m_id;
}

void Todo::copy(const Todo &rhs)
{
    setId(rhs.m_id);
    setTitle(rhs.m_title);
    setDescription(rhs.m_description);
    setEndDate(rhs.m_endDate);
    setCompleted(rhs.m_isCompleted);
    setOwner(rhs.m_owner);
    setWorkers(rhs.m_workers);
}

void Todo::copy(const QSharedPointer<Todo> &rhs)
{
    copy(*rhs.data());
}

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

void Todo::setId(int id) {
    SetProperty(id, m_id, [&]{ emit idChanged(id); });
}

void Todo::setTitle(const QString &title) {
    SetProperty(title, m_title, [&]{ emit titleChanged(title); });
}

void Todo::setDescription(const QString &description) {
    SetProperty(description, m_description, [&]{ emit descriptionChanged(description); });
}

void Todo::setEndDate(const QDateTime &endDate) {
    SetProperty(endDate, m_endDate, [&]{ emit endDateChanged(endDate); });
}

void Todo::setCompleted(bool isCompleted) {
    SetProperty(isCompleted, m_isCompleted, [&]{ emit completionChanged(isCompleted); });
}

void Todo::setOwner(const Person::Ptr &owner)
{
    SetProperty(owner, m_owner, [&]{ emit ownerChanged(owner); });
}

void Todo::setWorkers(const QList<Person::Ptr> &workers)
{
    SetProperty(workers, m_workers, [&]{ emit workersChanged(workers); });
}
