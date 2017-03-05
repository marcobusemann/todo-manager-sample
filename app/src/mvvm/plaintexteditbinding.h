#pragma once

#include <QByteArray>
#include <QDebug>

class QPlainTextEdit;
class PlainTextEditBinding: public QObject
{
    Q_OBJECT

public:
    static PlainTextEditBinding *factory(QPlainTextEdit *edit, QSharedPointer<QObject> item, const QString &propertyName);
    static PlainTextEditBinding *factory(QPlainTextEdit *edit, QObject *item, const QString &propertyName);

public:
    PlainTextEditBinding(QPlainTextEdit *edit, QObject *item, const QString &propertyName);

private slots:
    void updateModelValue();
    void updateViewValue(const QString &value);

private:
    QObject *m_item;
    QString m_propertyName;
    QPlainTextEdit *m_edit;

    void initialize();
    void bindModel();
    void bindView();
};