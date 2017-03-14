#pragma once

#include <QSharedPointer>
#include <QObject>

class QAbstractItemModel;
class QComboBox;
class ComboBoxBinding: public QObject
{
    Q_OBJECT

public:
    static ComboBoxBinding *factory(
        QComboBox *edit,
        QAbstractItemModel *itemModel,
        QSharedPointer<QObject> item,
        const QString &propertyName)
    {
        return new ComboBoxBinding(edit, itemModel, item.data(), propertyName);
    }

    static ComboBoxBinding *factory(
        QComboBox *edit,
        QAbstractItemModel *itemModel,
        QObject *item,
        const QString &propertyName)
    {
        return new ComboBoxBinding(edit, itemModel, item, propertyName);
    }

public:
    ComboBoxBinding(
        QComboBox *edit,
        QAbstractItemModel *itemModel,
        QObject *item,
        const QString &propertyName);

private slots:
    void updateModelValue(int index);
    void updateViewValue();
    void initialize();

private:
    QObject *m_item;
    QString m_propertyName;
    QComboBox *m_edit;
    bool m_hasBeenInitialized;

    void bindModel();
    void bindView();
};