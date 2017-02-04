#include "sqlitedatetimeconverter.h"
#include <QDateTime>

SQLiteDateTimeConverter::Ptr SQLiteDateTimeConverter::factory()
{
    return Ptr(new SQLiteDateTimeConverter());
}

QVariant SQLiteDateTimeConverter::convert(const QVariant &value) const
{
    QVariant result;
    if (value.type() == QVariant::Type::DateTime)
        result = value.toDateTime().toString(Qt::DateFormat::ISODate);
    else
        result = QDateTime::fromString(value.toString(), Qt::DateFormat::ISODate);
    return result;
}
