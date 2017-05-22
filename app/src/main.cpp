#include <QApplication>
#include <QFile>
#include <QFileInfo>

#include <QMetaOrm/DatabaseFactory.h>
#include <QMetaOrm/SessionFactory.h>
#include <QMetaOrm/Logger.h>

#include <dal/sqlitebooleanconverter.h>
#include <dal/sqlitedatetimeconverter.h>

#include <views/mainwindow.h>

#include <data/todo.h>

class SQLiteDatabaseFactory : public QMetaOrm::DatabaseFactory
{
public:
    static Ptr factory() {
        return Ptr(new SQLiteDatabaseFactory());
    }

    virtual QSqlDatabase createDatabase(const QString &name) const override {
        static const QString FILE = "Test.db";

        if (!QSqlDatabase::contains(name)) {
            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", name);
            db.setDatabaseName(FILE);

            QFileInfo file(FILE);

            //if (QFile::exists(FILE))
            //    QFile::remove(FILE);

            if (!QFile::exists(FILE)) {
                bool okOpen = db.open();
                Q_ASSERT_X(okOpen, __FUNCTION__, db.lastError().text().toLocal8Bit());

                QFile schemaFile(":/schema.sql");
                bool okOpenSchemaFile = schemaFile.open(QFile::ReadOnly);
                Q_ASSERT_X(okOpenSchemaFile, __FUNCTION__, db.lastError().text().toLocal8Bit());

                auto scriptParts = QString::fromLocal8Bit(schemaFile.readAll()).split(";", QString::SplitBehavior::SkipEmptyParts);
                for (auto part : scriptParts) {
                    part = part.trimmed();
                    if (part.isEmpty())
                        continue;
                    QSqlQuery query(db);
                    bool okExec = query.exec(part);
                    Q_ASSERT_X(okExec, __FUNCTION__, query.lastError().text().toLocal8Bit());
                }

                db.commit();
                schemaFile.close();
            }
        }
        return QSqlDatabase::database(name, false);
    }
};

#include <moderngrids\modelbuilder.h>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto databaseFactory = SQLiteDatabaseFactory::factory();
    auto logger = QMetaOrm::StandardQtLogger::factory();
    auto sessionFactory = QMetaOrm::DefaultSessionFactory::factory(databaseFactory, logger);
    sessionFactory->getConverterStore()->registerConverter("BooleanConverter", SQLiteBooleanConverter::factory());
    sessionFactory->getConverterStore()->registerConverter("DateTimeConverter", SQLiteDateTimeConverter::factory());

    MainWindow w(sessionFactory);
    w.show();

    /*
    auto sourceModel = new QStandardItemModel();

    QStandardItem *item1 = new QStandardItem("Test");
    sourceModel->appendRow(item1);

    QStandardItem *item2 = new QStandardItem("Test2");
    sourceModel->appendRow(item2);

    auto sortModel = new QSortFilterProxyModel();
    sortModel->setSourceModel(sourceModel);

    auto outerModel = new SourceMappingModelDecorator();
    outerModel->setSourceModel(sortModel);
    outerModel->flags(QModelIndex());

    auto valueItem1_1 = outerModel->index(0, 0).data(Qt::DisplayRole).toString().toStdString();
    auto valueItem2_1 = outerModel->index(1, 0).data(Qt::DisplayRole).toString().toStdString();

    sortModel->sort(0, Qt::DescendingOrder);

    auto indexValueItem1_2 = outerModel->index(0, 0);
    auto valueItem1_2 = indexValueItem1_2.data(Qt::DisplayRole).toString().toStdString();
    auto valueItem2_2 = outerModel->index(1, 0).data(Qt::DisplayRole).toString().toStdString();

    auto mappedindexValueItem1_2 = outerModel->mapToSource(indexValueItem1_2);
    auto mappedValueItem2 = mappedindexValueItem1_2.data(Qt::DisplayRole).toString().toStdString();
    */
    return a.exec();
}
