#pragma once

#include <QByteArray>
#include <QDebug>

class QLineEdit;
class LineEditBinding: public QObject
{
    Q_OBJECT

public:
    static LineEditBinding *factory(QLineEdit *edit, QSharedPointer<QObject> item, const QString &propertyName);
    static LineEditBinding *factory(QLineEdit *edit, QObject *item, const QString &propertyName);

public:
    LineEditBinding(QLineEdit *edit, QObject *item, const QString &propertyName);

private slots:
    void updateModelValue(const QString &value);
    void updateViewValue(const QString &value);

private:
    QObject *m_item;
    QString m_propertyName;
    QLineEdit *m_edit;

    void initialize();
    void bindModel();
    void bindView();
};