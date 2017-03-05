#include <mvvm/plaintexteditbinding.h>
#include <QMetaProperty>
#include <QPlainTextEdit>

PlainTextEditBinding *PlainTextEditBinding::factory(QPlainTextEdit *edit, QSharedPointer<QObject> item, const QString &propertyName) {
    return new PlainTextEditBinding(edit, item.data(), propertyName);
}

PlainTextEditBinding *PlainTextEditBinding::factory(QPlainTextEdit *edit, QObject *item, const QString &propertyName) {
    return new PlainTextEditBinding(edit, item, propertyName);
}

PlainTextEditBinding::PlainTextEditBinding(
    QPlainTextEdit *edit,
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

void PlainTextEditBinding::updateModelValue() {
    auto value = m_edit->toPlainText();
    m_item->setProperty(m_propertyName.toLocal8Bit(), value);
}

void PlainTextEditBinding::updateViewValue(const QString &value) {
    auto currentValue = m_edit->toPlainText();
    if (currentValue == value) return;
    m_edit->setPlainText(value);
}

void PlainTextEditBinding::initialize() {
    auto currentValue = m_item->property(m_propertyName.toLocal8Bit());
    if (currentValue.isValid())
        m_edit->setPlainText(currentValue.toString());
}

void PlainTextEditBinding::bindModel() {
    auto itemPropertyIndex = m_item->metaObject()->indexOfProperty(m_propertyName.toLocal8Bit());
    Q_ASSERT_X(itemPropertyIndex >= 0, __FUNCTION__, QString("property %1 does not exist").arg(m_propertyName).toLocal8Bit());

    auto itemProperty = m_item->metaObject()->property(itemPropertyIndex);

    auto binderSlotIndex = metaObject()->indexOfMethod("updateViewValue(QString)");
    Q_ASSERT_X(binderSlotIndex >= 0, __FUNCTION__, "binding method not found");

    auto binderSlot = metaObject()->method(binderSlotIndex);

    bool ok = connect(m_item, itemProperty.notifySignal(), this, binderSlot);
    Q_ASSERT_X(ok, __FUNCTION__, "binding model failed");
}

void PlainTextEditBinding::bindView() {
    bool ok = connect(m_edit, &QPlainTextEdit::textChanged, this, &PlainTextEditBinding::updateModelValue);
    Q_ASSERT_X(ok, __FUNCTION__, "binding view failed");
}