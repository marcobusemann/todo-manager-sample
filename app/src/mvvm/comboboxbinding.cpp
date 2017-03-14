#include <mvvm/comboboxbinding.h>

#include <QMetaProperty>
#include <QComboBox>
#include <QDebug>

ComboBoxBinding::ComboBoxBinding(
    QComboBox *edit,
    QAbstractItemModel *itemModel,
    QObject *item,
    const QString &propertyName)
    : QObject(edit)
    , m_item(item)
    , m_propertyName(propertyName)
    , m_edit(edit)
    , m_hasBeenInitialized(false)
{
    m_edit->setModel(itemModel);

    connect(itemModel, SIGNAL(rowsInserted(const QModelIndex &, int, int)), SLOT(initialize()));

    initialize();
    bindModel();
    bindView();
}

void ComboBoxBinding::updateModelValue(int index) {
    if (!m_hasBeenInitialized)
        return;
    qDebug() << "updateModelValue" << index;
    auto modelIndex = m_edit->model()->index(index, 0);
    auto value = modelIndex.isValid() ? 
        m_edit->model()->data(modelIndex, Qt::UserRole) :
        QVariant();
    m_item->setProperty(m_propertyName.toLocal8Bit(), value);
}

void ComboBoxBinding::updateViewValue() {
    qDebug() << "updateViewValue";
    auto currentValue = m_item->property(m_propertyName.toLocal8Bit());
    auto model = m_edit->model();

    if (currentValue.isValid() && !currentValue.isNull() && model->rowCount() > 0)
    {
        auto matchingValues = model->match(model->index(0, 0), Qt::UserRole, currentValue);

        if (!matchingValues.isEmpty())
        {
            bool differentIndex = matchingValues[0].row() != m_edit->currentIndex();
            if (differentIndex)
            {
                int foundIndex = matchingValues[0].row();
                m_edit->blockSignals(true);
                m_edit->setCurrentIndex(foundIndex);
                m_edit->blockSignals(false);
                qDebug() << "updated current index: " << foundIndex;
            }
        }
        else 
            updateModelValue(0);
    }
}

void ComboBoxBinding::initialize() {
    qDebug() << "initialize";
    auto currentValue = m_item->property(m_propertyName.toLocal8Bit());
    auto model = m_edit->model();
    if (currentValue.isValid() && !currentValue.isNull() && model->rowCount() > 0 && !m_hasBeenInitialized)
    {
        auto matchingValues = model->match(model->index(0, 0), Qt::UserRole, currentValue);

        if (!matchingValues.isEmpty())
        {
            int foundIndex = matchingValues[0].row();
            m_edit->blockSignals(true);
            m_edit->setCurrentIndex(foundIndex);
            m_edit->blockSignals(false);
            qDebug() << "initialize: " << foundIndex;
            m_hasBeenInitialized = true;
        }
        else {
            m_hasBeenInitialized = true;
            updateModelValue(0);
        }
    }
}

void ComboBoxBinding::bindModel() {
    auto itemPropertyIndex = m_item->metaObject()->indexOfProperty(m_propertyName.toLocal8Bit());
    Q_ASSERT_X(itemPropertyIndex >= 0, __FUNCTION__, QString("property %1 does not exist").arg(m_propertyName).toLocal8Bit());

    auto itemProperty = m_item->metaObject()->property(itemPropertyIndex);

    auto binderSlotIndex = metaObject()->indexOfMethod("updateViewValue()");
    Q_ASSERT_X(binderSlotIndex >= 0, __FUNCTION__, "binding method not found");

    auto binderSlot = metaObject()->method(binderSlotIndex);

    bool ok = connect(m_item, itemProperty.notifySignal(), this, binderSlot);
    Q_ASSERT_X(ok, __FUNCTION__, "binding model failed");
}

void ComboBoxBinding::bindView() {
    bool ok = connect(m_edit, SIGNAL(currentIndexChanged(int)), this, SLOT(updateModelValue(int)));
    Q_ASSERT_X(ok, __FUNCTION__, "binding view failed");
}