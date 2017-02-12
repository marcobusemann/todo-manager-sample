#pragma once

#include <QMetaOrm/Converter.h>

class SQLiteBooleanConverter : public QMetaOrm::Converter {
public:
    static Ptr factory();
    QVariant convert(const QVariant &value) const;
};
