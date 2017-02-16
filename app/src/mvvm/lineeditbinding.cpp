#include <mvvm/lineeditbinding.h>
#include <QMetaProperty>
#include <QLineEdit>

LineEditBinding *LineEditBinding::factory(QLineEdit *edit, QSharedPointer<QObject> item, const QString &propertyName) {
    return new LineEditBinding(edit, item.data(), propertyName);
}

LineEditBinding *LineEditBinding::factory(QLineEdit *edit, QObject *item, const QString &propertyName) {
    return new LineEditBinding(edit, item, propertyName);
}

LineEditBinding::LineEditBinding(
    QLineEdit *edit,
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

void LineEditBinding::updateModelValue(const QString &value) {
    m_item->setProperty(m_propertyName.toLocal8Bit(), value);
}

void LineEditBinding::updateViewValue(const QString &value) {
    auto currentValue = m_edit->text();
    if (currentValue == value) return;
    m_edit->setText(value);
}

void LineEditBinding::initialize() {
    auto currentValue = m_item->property(m_propertyName.toLocal8Bit());
    if (currentValue.isValid())
        m_edit->setText(currentValue.toString());
}

void LineEditBinding::bindModel() {
    auto itemPropertyIndex = m_item->metaObject()->indexOfProperty(m_propertyName.toLocal8Bit());
    Q_ASSERT_X(itemPropertyIndex >= 0, __FUNCTION__, "property not found");

    auto itemProperty = m_item->metaObject()->property(itemPropertyIndex);

    auto binderSlotIndex = metaObject()->indexOfMethod("updateViewValue(QString)");
    Q_ASSERT_X(binderSlotIndex >= 0, __FUNCTION__, "binding method not found");

    auto binderSlot = metaObject()->method(binderSlotIndex);

    bool ok = connect(m_item, itemProperty.notifySignal(), this, binderSlot);
    Q_ASSERT_X(ok, __FUNCTION__, "binding model failed");
}

void LineEditBinding::bindView() {
    bool ok = connect(m_edit, &QLineEdit::textChanged, this, &LineEditBinding::updateModelValue);
    Q_ASSERT_X(ok, __FUNCTION__, "binding view failed");
}