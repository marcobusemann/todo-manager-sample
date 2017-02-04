#include "sqlitebooleanconverter.h"

SQLiteBooleanConverter::Ptr SQLiteBooleanConverter::factory()
{
    return Ptr(new SQLiteBooleanConverter());
}

QVariant SQLiteBooleanConverter::convert(const QVariant &value) const
{
    QVariant result;
    if (value.type() == QVariant::Type::Bool)
        result = value.toBool() == true ? "T" : "F";
    else
        result = value.toString() == "T";
    return result;
}
