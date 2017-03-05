#include <mvvm/datetimeeditbinding.h>
#include <QMetaProperty>
#include <QDateTimeEdit>

DateTimeEditBinding *DateTimeEditBinding::factory(QDateTimeEdit *edit, QSharedPointer<QObject> item, const QString &propertyName) {
    return new DateTimeEditBinding(edit, item.data(), propertyName);
}

DateTimeEditBinding *DateTimeEditBinding::factory(QDateTimeEdit *edit, QObject *item, const QString &propertyName) {
    return new DateTimeEditBinding(edit, item, propertyName);
}

DateTimeEditBinding::DateTimeEditBinding(
    QDateTimeEdit *edit,
    QObject *item,
    const QString &propertyName)
    : QObject(edit)
    , m_item(item)
    , m_propertyName(propertyName)
    , m_edit(edit)
{
    initialize();
    bindModel();
    bindView();
}

void DateTimeEditBinding::updateModelValue(const QDateTime &value) {
    m_item->setProperty(m_propertyName.toLocal8Bit(), value);
}

void DateTimeEditBinding::updateViewValue(const QDateTime &value) {
    auto currentValue = m_edit->dateTime();
    if (currentValue == value) return;
    m_edit->setDateTime(value);
}

void DateTimeEditBinding::initialize() {
    auto currentValue = m_item->property(m_propertyName.toLocal8Bit());
    if (currentValue.isValid())
        m_edit->setDateTime(currentValue.toDateTime());
}

void DateTimeEditBinding::bindModel() {
    auto itemPropertyIndex = m_item->metaObject()->indexOfProperty(m_propertyName.toLocal8Bit());
    Q_ASSERT_X(itemPropertyIndex >= 0, __FUNCTION__, QString("property %1 does not exist").arg(m_propertyName).toLocal8Bit());

    auto itemProperty = m_item->metaObject()->property(itemPropertyIndex);

    auto binderSlotIndex = metaObject()->indexOfMethod("updateViewValue(QDateTime)");
    Q_ASSERT_X(binderSlotIndex >= 0, __FUNCTION__, "binding method not found");

    auto binderSlot = metaObject()->method(binderSlotIndex);

    bool ok = connect(m_item, itemProperty.notifySignal(), this, binderSlot);
    Q_ASSERT_X(ok, __FUNCTION__, "binding model failed");
}

void DateTimeEditBinding::bindView() {
    bool ok = connect(m_edit, &QDateTimeEdit::dateTimeChanged, this, &DateTimeEditBinding::updateModelValue);
    Q_ASSERT_X(ok, __FUNCTION__, "binding view failed");
}