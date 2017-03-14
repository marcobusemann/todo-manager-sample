#pragma once

#include <QModelIndex>

class ModelLoggerPrivate;

/*!
*/
class ModelLogger
{
public:
    static ModelLogger &instance();
    ~ModelLogger();

    void logData(const QString &source, const QModelIndex &index, int role, const QVariant &result);
    void logRowCount(const QString &source, const QModelIndex &index, int result);
    void logColumnCount(const QString &source, const QModelIndex &index, int result);
    void logParent(const QString &source, const QModelIndex &index, const QModelIndex &result);
    void logIndex(const QString &source, int row, int column, const QModelIndex &parent, const QModelIndex &result);

    void setEnabled(bool enabled);

private:
    ModelLoggerPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(ModelLogger)
        ModelLogger();
};