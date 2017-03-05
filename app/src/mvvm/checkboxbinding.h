#pragma once

#include <QByteArray>
#include <QDebug>

class QCheckBox;
class CheckBoxBinding: public QObject
{
    Q_OBJECT

public:
    static CheckBoxBinding *factory(QCheckBox *edit, QSharedPointer<QObject> item, const QString &propertyName);
    static CheckBoxBinding *factory(QCheckBox *edit, QObject *item, const QString &propertyName);

public:
    CheckBoxBinding(QCheckBox *edit, QObject *item, const QString &propertyName);

private slots:
    void updateModelValue(bool value);
    void updateViewValue(bool value);

private:
    QObject *m_item;
    QString m_propertyName;
    QCheckBox *m_edit;

    void initialize();
    void bindModel();
    void bindView();
};