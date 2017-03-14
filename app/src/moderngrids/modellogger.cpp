#include "modellogger.h"
#include <QStringList>
#include <QDebug>

//-----------------------------------------------------------------------------
class ModelLoggerPrivate
{
public:
    QHash<QString, QString> lastData;
    QHash<QString, QString> lastRowCount;
    QHash<QString, QString> lastColumnCount;
    QHash<QString, QString> lastParentCount;
    QHash<QString, QString> lastIndex;
    bool enabled;
};

//-----------------------------------------------------------------------------
ModelLogger::ModelLogger()
    : d_ptr(new ModelLoggerPrivate())
{
    d_ptr->enabled = false;
}

//-----------------------------------------------------------------------------
ModelLogger::~ModelLogger()
{
    delete d_ptr;
}

//-----------------------------------------------------------------------------
ModelLogger &ModelLogger::instance()
{
    static ModelLogger logger;
    return logger;
}

//-----------------------------------------------------------------------------
void ModelLogger::setEnabled(bool enabled)
{
    d_ptr->enabled = enabled;
}

//-----------------------------------------------------------------------------
void ModelLogger::logData(const QString &source, const QModelIndex &index, int role, const QVariant &result)
{
    Q_D(ModelLogger);
    if (!d->enabled) return;
    QString data;
    QDebug(&data) << index << role << result;
    if (!d->lastData.contains(source) || d->lastData[source] != data)
    {
        d->lastData[source] = data;
        qDebug() << source << data;
    }
}

//-----------------------------------------------------------------------------
void ModelLogger::logRowCount(const QString &source, const QModelIndex &index, int result)
{
    Q_D(ModelLogger);
    if (!d->enabled) return;
    QString data;
    QDebug(&data) << index << result;
    if (!d->lastRowCount.contains(source) || d->lastRowCount[source] != data)
    {
        d->lastRowCount[source] = data;
        qDebug() << source << data;
    }
}

//-----------------------------------------------------------------------------
void ModelLogger::logColumnCount(const QString &source, const QModelIndex &index, int result)
{
    Q_D(ModelLogger);
    if (!d->enabled) return;
    QString data;
    QDebug(&data) << index << result;
    if (!d->lastColumnCount.contains(source) || d->lastColumnCount[source] != data)
    {
        d->lastColumnCount[source] = data;
        qDebug() << source << data;
    }
}

//-----------------------------------------------------------------------------
void ModelLogger::logParent(const QString &source, const QModelIndex &index, const QModelIndex &result)
{
    Q_D(ModelLogger);
    if (!d->enabled) return;
    QString data;
    QDebug(&data) << index << result;
    if (!d->lastParentCount.contains(source) || d->lastParentCount[source] != data)
    {
        d->lastParentCount[source] = data;
        qDebug() << source << data;
    }
}

//-----------------------------------------------------------------------------
void ModelLogger::logIndex(const QString &source, int row, int column, const QModelIndex &parent, const QModelIndex &result)
{
    Q_D(ModelLogger);
    if (!d->enabled) return;
    QString data;
    QDebug(&data) << row << column << parent << result;
    if (!d->lastIndex.contains(source) || d->lastIndex[source] != data)
    {
        d->lastIndex[source] = data;
        qDebug() << source << data;
    }
}
