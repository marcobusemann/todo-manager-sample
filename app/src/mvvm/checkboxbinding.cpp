#include <mvvm/checkboxbinding.h>
#include <QMetaProperty>
#include <QCheckBox>

CheckBoxBinding *CheckBoxBinding::factory(QCheckBox *edit, QSharedPointer<QObject> item, const QString &propertyName) {
    return new CheckBoxBinding(edit, item.data(), propertyName);
}

CheckBoxBinding *CheckBoxBinding::factory(QCheckBox *edit, QObject *item, const QString &propertyName) {
    return new CheckBoxBinding(edit, item, propertyName);
}

CheckBoxBinding::CheckBoxBinding(
    QCheckBox *edit,
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

void CheckBoxBinding::updateModelValue(bool value) {
    m_item->setProperty(m_propertyName.toLocal8Bit(), value);
}

void CheckBoxBinding::updateViewValue(bool value) {
    auto currentValue = m_edit->isChecked();
    if (currentValue == value) return;
    m_edit->setChecked(value);
}

void CheckBoxBinding::initialize() {
    auto currentValue = m_item->property(m_propertyName.toLocal8Bit());
    if (currentValue.isValid())
        m_edit->setChecked(currentValue.toBool());
}

void CheckBoxBinding::bindModel() {
    auto itemPropertyIndex = m_item->metaObject()->indexOfProperty(m_propertyName.toLocal8Bit());
    Q_ASSERT_X(itemPropertyIndex >= 0, __FUNCTION__, QString("property %1 does not exist").arg(m_propertyName).toLocal8Bit());

    auto itemProperty = m_item->metaObject()->property(itemPropertyIndex);

    auto binderSlotIndex = metaObject()->indexOfMethod("updateViewValue(bool)");
    Q_ASSERT_X(binderSlotIndex >= 0, __FUNCTION__, "binding method not found");

    auto binderSlot = metaObject()->method(binderSlotIndex);

    bool ok = connect(m_item, itemProperty.notifySignal(), this, binderSlot);
    Q_ASSERT_X(ok, __FUNCTION__, "binding model failed");
}

void CheckBoxBinding::bindView() {
    bool ok = connect(m_edit, &QCheckBox::toggled, this, &CheckBoxBinding::updateModelValue);
    Q_ASSERT_X(ok, __FUNCTION__, "binding view failed");
}