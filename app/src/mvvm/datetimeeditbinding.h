#pragma once

#include <QDateTime>
#include <QObject>

class QDateTimeEdit;
class DateTimeEditBinding: public QObject
{
    Q_OBJECT

public:
    static DateTimeEditBinding *factory(QDateTimeEdit *edit, QSharedPointer<QObject> item, const QString &propertyName);
    static DateTimeEditBinding *factory(QDateTimeEdit *edit, QObject *item, const QString &propertyName);

public:
    DateTimeEditBinding(QDateTimeEdit *edit, QObject *item, const QString &propertyName);

private slots:
    void updateModelValue(const QDateTime &value);
    void updateViewValue(const QDateTime &value);

private:
    QObject *m_item;
    QString m_propertyName;
    QDateTimeEdit *m_edit;

    void initialize();
    void bindModel();
    void bindView();
};