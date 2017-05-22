#pragma once

#include "decorators/datamodeldecorator.h"
#include "decorators/headerdatamodeldecorator.h"
#include "decorators/alternaterowshadingmodeldecorator.h"
#include "decorators/checkboxmultimarkmodeldecorator.h"
#include "decorators/columnmodeldecorator.h"
#include "decorators/objectdatamodeldecorator.h"
#include "decorators/fetchmoredecorator.h"

#include "qobservablevariantlistproxy.h"
#include "observablevariantlistmodel.h"

#include "observableset.h"
#include "qobservablelist.h"

#include <QSortFilterProxyModel>
#include <QAbstractItemModel>
#include <QIcon>
#include <QSet>

class ModelBuilderCommon;

class ModelBuilderData final
{
public:
    ModelBuilderData(QAbstractItemModel *base, QObject *parent)
        : m_column(-1)
        , m_role(-1)
        , m_exclusions(QSet<int>() << Qt::UserRole)
        , m_base(base)
        , m_parent(parent)
    {}

    ModelBuilderData &forColumn(int column)
    {
        m_column = column;
        return *this;
    }

    ModelBuilderData &forRole(int role)
    {
        m_role = role;
        return *this;
    }

    ModelBuilderData &withRoleExclusions(const QSet<int> &exclusions)
    {
        m_exclusions = exclusions;
        return *this;
    }

    ModelBuilderCommon withHandler(DataModelDecorator::Handler handler);

private:
    QObject * const m_parent;
    int m_column;
    int m_role;
    QSet<int> m_exclusions;
    QAbstractItemModel *m_base;
};

class ModelBuilderHeaderData final
{
public:
    ModelBuilderHeaderData(QAbstractItemModel *base, QObject *parent)
        : m_section(-1)
        , m_role(-1)
        , m_orientation(Qt::Horizontal)
        , m_base(base)
        , m_parent(parent)
    {}

    ModelBuilderHeaderData &forSection(int section)
    {
        m_section = section;
        return *this;
    }

    ModelBuilderHeaderData &forRole(int role)
    {
        m_role = role;
        return *this;
    }

    ModelBuilderHeaderData &withOrientation(Qt::Orientation orientation)
    {
        m_orientation = orientation;
        return *this;
    }

    ModelBuilderCommon withHandler(HeaderDataModelDecorator::Handler handler);

private:
    QObject * const m_parent;
    int m_section;
    int m_role;
    Qt::Orientation m_orientation;
    QAbstractItemModel *m_base;
};

/*!
   This decorator implements mapFromSource and mapToSource to map to the root model instead of the next source.
*/
class SourceMappingModelDecorator : public QAbstractProxyModel
{
public: 
   SourceMappingModelDecorator(QObject *parent)
      : QAbstractProxyModel(parent)
   {}

   virtual QModelIndex buddy(const QModelIndex &index) const override
   {
      return sourceModel()->buddy(index);
   }

   virtual bool canFetchMore(const QModelIndex &parent) const override
   {
      return sourceModel()->canFetchMore(parent);
   }

   virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override
   {
      return sourceModel()->dropMimeData(data, action, row, column, parent);
   }

   virtual void fetchMore(const QModelIndex &parent) override
   {
      return sourceModel()->fetchMore(parent);
   }

   virtual Qt::ItemFlags flags(const QModelIndex &index) const override
   {
      return sourceModel()->flags(index);
   }

   virtual bool hasChildren(const QModelIndex &parent = QModelIndex()) const override
   {
      return sourceModel()->hasChildren(parent);
   }

   virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override
   {
      return sourceModel()->headerData(section, orientation, role);
   }

   virtual bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override
   {
      return sourceModel()->insertColumns(column, count, parent);
   }

   virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override
   {
      return sourceModel()->insertRows(row, count, parent);
   }

   virtual QMap<int, QVariant> itemData(const QModelIndex &index) const override
   {
      return sourceModel()->itemData(index);
   }

   virtual QModelIndexList match(const QModelIndex &start, int role, const QVariant &value, int hits = 1, Qt::MatchFlags flags = Qt::MatchFlags(Qt::MatchStartsWith | Qt::MatchWrap)) const override
   {
      return sourceModel()->match(start, role, value, hits, flags);
   }

   virtual QMimeData *mimeData(const QModelIndexList &indexes) const override
   {
      return sourceModel()->mimeData(indexes);
   }

   virtual QStringList mimeTypes() const override
   {
      return sourceModel()->mimeTypes();
   }

   virtual bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override
   {
      return sourceModel()->removeColumns(column, count, parent);
   }

   virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override
   {
      return sourceModel()->removeRows(row, count, parent);
   }

   virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override
   {
      return sourceModel()->setData(index, value, role);
   }

   virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override
   {
      return sourceModel()->setHeaderData(section, orientation, value, role);
   }

   virtual bool setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles) override
   {
      return sourceModel()->setItemData(index, roles);
   }

   virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override
   {
      return sourceModel()->sort(column, order);
   }

   virtual QSize span(const QModelIndex &index) const override
   {
      return sourceModel()->span(index);
   }

   virtual Qt::DropActions supportedDropActions() const override
   {
      return sourceModel()->supportedDropActions();
   }

   virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override
   {
      return sourceModel()->columnCount(parent);
   }

   virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
   {
      return sourceModel()->data(index, role);
   }

   virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override
   {
      return sourceModel()->index(row, column, parent);
   }

   virtual QModelIndex parent(const QModelIndex & index) const override
   {
      return sourceModel()->parent(index);
   }

   virtual int rowCount(const QModelIndex & parent = QModelIndex()) const override
   {
      return sourceModel()->rowCount(parent);
   }

    virtual QModelIndex	mapFromSource(const QModelIndex &sourceIndex) const override
    {
       const QAbstractProxyModel *origin = this;
       QModelIndex result = sourceIndex;

       while ((origin = dynamic_cast<QAbstractProxyModel*>(origin->sourceModel())) != nullptr)
          result = origin->mapFromSource(result);

       return result;
    }

    virtual QModelIndex	mapToSource(const QModelIndex &proxyIndex) const override
    {
       const QAbstractProxyModel *origin = this;
       QModelIndex result = proxyIndex;

       while ((origin = dynamic_cast<QAbstractProxyModel*>(origin->sourceModel())) != nullptr)
          result = origin->mapToSource(result);

       return result;
    }

    virtual void setSourceModel(QAbstractItemModel *newSourceModel) override
    {
       beginResetModel();

       if (sourceModel()) {
          disconnect(sourceModel(), SIGNAL(rowsAboutToBeInserted(QModelIndex, int, int)),
             this, SIGNAL(rowsAboutToBeInserted(QModelIndex, int, int)));
          disconnect(sourceModel(), SIGNAL(rowsInserted(QModelIndex, int, int)),
             this, SIGNAL(rowsInserted(QModelIndex, int, int)));
          disconnect(sourceModel(), SIGNAL(rowsAboutToBeRemoved(QModelIndex, int, int)),
             this, SIGNAL(rowsAboutToBeRemoved(QModelIndex, int, int)));
          disconnect(sourceModel(), SIGNAL(rowsRemoved(QModelIndex, int, int)),
             this, SIGNAL(rowsRemoved(QModelIndex, int, int)));
          disconnect(sourceModel(), SIGNAL(rowsAboutToBeMoved(QModelIndex, int, int, QModelIndex, int)),
             this, SIGNAL(rowsAboutToBeMoved(QModelIndex, int, int, QModelIndex, int)));
          disconnect(sourceModel(), SIGNAL(rowsMoved(QModelIndex, int, int, QModelIndex, int)),
             this, SIGNAL(rowsMoved(QModelIndex, int, int, QModelIndex, int)));
          disconnect(sourceModel(), SIGNAL(columnsAboutToBeInserted(QModelIndex, int, int)),
             this, SIGNAL(columnsAboutToBeInserted(QModelIndex, int, int)));
          disconnect(sourceModel(), SIGNAL(columnsInserted(QModelIndex, int, int)),
             this, SIGNAL(columnsInserted(QModelIndex, int, int)));
          disconnect(sourceModel(), SIGNAL(columnsAboutToBeRemoved(QModelIndex, int, int)),
             this, SIGNAL(columnsAboutToBeRemoved(QModelIndex, int, int)));
          disconnect(sourceModel(), SIGNAL(columnsRemoved(QModelIndex, int, int)),
             this, SIGNAL(columnsRemoved(QModelIndex, int, int)));
          disconnect(sourceModel(), SIGNAL(columnsAboutToBeMoved(QModelIndex, int, int, QModelIndex, int)),
             this, SIGNAL(columnsAboutToBeMoved(QModelIndex, int, int, QModelIndex, int)));
          disconnect(sourceModel(), SIGNAL(columnsMoved(QModelIndex, int, int, QModelIndex, int)),
             this, SIGNAL(columnsMoved(QModelIndex, int, int, QModelIndex, int)));
          disconnect(sourceModel(), SIGNAL(modelAboutToBeReset()),
             this, SIGNAL(modelAboutToBeReset()));
          disconnect(sourceModel(), SIGNAL(modelReset()),
             this, SIGNAL(modelReset()));
          disconnect(sourceModel(), SIGNAL(dataChanged(QModelIndex, QModelIndex, QVector<int>)),
             this, SIGNAL(dataChanged(QModelIndex, QModelIndex, QVector<int>)));
          disconnect(sourceModel(), SIGNAL(headerDataChanged(Qt::Orientation, int, int)),
             this, SIGNAL(headerDataChanged(Qt::Orientation, int, int)));
          disconnect(sourceModel(), SIGNAL(layoutAboutToBeChanged(QList<QPersistentModelIndex>, QAbstractItemModel::LayoutChangeHint)),
             this, SIGNAL(layoutAboutToBeChanged(QList<QPersistentModelIndex>, QAbstractItemModel::LayoutChangeHint)));
          disconnect(sourceModel(), SIGNAL(layoutChanged(QList<QPersistentModelIndex>, QAbstractItemModel::LayoutChangeHint)),
             this, SIGNAL(layoutChanged(QList<QPersistentModelIndex>, QAbstractItemModel::LayoutChangeHint)));
       }

       QAbstractProxyModel::setSourceModel(newSourceModel);

       if (sourceModel()) {
          connect(sourceModel(), SIGNAL(rowsAboutToBeInserted(QModelIndex, int, int)),
             SIGNAL(rowsAboutToBeInserted(QModelIndex, int, int)));
          connect(sourceModel(), SIGNAL(rowsInserted(QModelIndex, int, int)),
             SIGNAL(rowsInserted(QModelIndex, int, int)));
          connect(sourceModel(), SIGNAL(rowsAboutToBeRemoved(QModelIndex, int, int)),
             SIGNAL(rowsAboutToBeRemoved(QModelIndex, int, int)));
          connect(sourceModel(), SIGNAL(rowsRemoved(QModelIndex, int, int)),
             SIGNAL(rowsRemoved(QModelIndex, int, int)));
          connect(sourceModel(), SIGNAL(rowsAboutToBeMoved(QModelIndex, int, int, QModelIndex, int)),
             SIGNAL(rowsAboutToBeMoved(QModelIndex, int, int, QModelIndex, int)));
          connect(sourceModel(), SIGNAL(rowsMoved(QModelIndex, int, int, QModelIndex, int)),
             SIGNAL(rowsMoved(QModelIndex, int, int, QModelIndex, int)));
          connect(sourceModel(), SIGNAL(columnsAboutToBeInserted(QModelIndex, int, int)),
             SIGNAL(columnsAboutToBeInserted(QModelIndex, int, int)));
          connect(sourceModel(), SIGNAL(columnsInserted(QModelIndex, int, int)),
             SIGNAL(columnsInserted(QModelIndex, int, int)));
          connect(sourceModel(), SIGNAL(columnsAboutToBeRemoved(QModelIndex, int, int)),
             SIGNAL(columnsAboutToBeRemoved(QModelIndex, int, int)));
          connect(sourceModel(), SIGNAL(columnsRemoved(QModelIndex, int, int)),
             SIGNAL(columnsRemoved(QModelIndex, int, int)));
          connect(sourceModel(), SIGNAL(columnsAboutToBeMoved(QModelIndex, int, int, QModelIndex, int)),
             SIGNAL(columnsAboutToBeMoved(QModelIndex, int, int, QModelIndex, int)));
          connect(sourceModel(), SIGNAL(columnsMoved(QModelIndex, int, int, QModelIndex, int)),
             SIGNAL(columnsMoved(QModelIndex, int, int, QModelIndex, int)));
          connect(sourceModel(), SIGNAL(modelAboutToBeReset()),
             SIGNAL(modelAboutToBeReset()));
          connect(sourceModel(), SIGNAL(modelReset()),
             SIGNAL(modelReset()));
          connect(sourceModel(), SIGNAL(dataChanged(QModelIndex, QModelIndex, QVector<int>)),
             SIGNAL(dataChanged(QModelIndex, QModelIndex, QVector<int>)));
          connect(sourceModel(), SIGNAL(headerDataChanged(Qt::Orientation, int, int)),
             SIGNAL(headerDataChanged(Qt::Orientation, int, int)));
          connect(sourceModel(), SIGNAL(layoutAboutToBeChanged(QList<QPersistentModelIndex>, QAbstractItemModel::LayoutChangeHint)),
             SIGNAL(layoutAboutToBeChanged(QList<QPersistentModelIndex>, QAbstractItemModel::LayoutChangeHint)));
          connect(sourceModel(), SIGNAL(layoutChanged(QList<QPersistentModelIndex>, QAbstractItemModel::LayoutChangeHint)),
             SIGNAL(layoutChanged(QList<QPersistentModelIndex>, QAbstractItemModel::LayoutChangeHint)));
       }

       endResetModel();
    }
};

class ModelBuilderCommon final
{
public:
    ModelBuilderCommon(QAbstractItemModel *base, QObject *parent)
        : m_base(base)
        , m_parent(parent)
    {
    }

    template <class T>
    ModelBuilderCommon &withObjectData(std::function<QVariant(int, const T &)> handler)
    {
        return embeddNewModel(ObjectDataModelDecorator::ofType<T>(handler, m_parent));
    }

    template <class T>
    ModelBuilderCommon &withProperty(int column, const QString &propertyName)
    {
        return embeddNewModel(ObjectPropertyColumnModelDecorator::ofType<T>(column, propertyName, m_parent));
    }

    ModelBuilderCommon &withData(DataModelDecorator::Handler handler)
    {
        return embeddNewModel(new DataModelDecorator(handler, m_parent));
    }

    ModelBuilderCommon &withData(DataModelDecorator::RawHandler handler)
    {
        return embeddNewModel(new DataModelDecorator(handler, m_parent));
    }

    ModelBuilderCommon &withData(int role, DataModelDecorator::Handler handler)
    {
        return embeddNewModel(new RoleBasedDataModelDecorator(role, handler, m_parent));
    }

    ModelBuilderCommon &withData(int role, DataModelDecorator::RawHandler handler)
    {
        return embeddNewModel(new RoleBasedDataModelDecorator(role, handler, m_parent));
    }

    ModelBuilderData withData()
    {
        return ModelBuilderData(m_base, m_parent);
    }

    ModelBuilderCommon &withFetchMore(FetchMoreModelDecorator::CanFetchMoreHandler canFetchMoreHandler, FetchMoreModelDecorator::FetchMoreHandler fetchMoreHandler)
    {
        return embeddNewModel(new FetchMoreModelDecorator(canFetchMoreHandler, fetchMoreHandler, m_parent));
    }

    ModelBuilderCommon &withHorizontalHeaderData(HeaderDataModelDecorator::Handler handler)
    {
        return embeddNewModel(new HeaderDataModelDecorator(Qt::Horizontal, handler, m_parent));
    }

    ModelBuilderCommon &withVerticalHeaderData(HeaderDataModelDecorator::Handler handler)
    {
        return embeddNewModel(new HeaderDataModelDecorator(Qt::Horizontal, handler, m_parent));
    }

    ModelBuilderHeaderData withHeaderData()
    {
        return ModelBuilderHeaderData(m_base, m_parent);
    }

    ModelBuilderCommon &withAlternateRowShading(const QColor &colorA, const QColor &colorB)
    {
        return embeddNewModel(new AlternateRowShadingModelDecorator(colorA, colorB, m_parent));
    }

    ModelBuilderCommon &withSortAndFilter(std::function<void(QSortFilterProxyModel*)> handler)
    {
       auto sortFilterModel = new QSortFilterProxyModel(m_parent);
       handler(sortFilterModel);
       return embeddNewModel(sortFilterModel);
    }

    ModelBuilderCommon &withCheckboxMultiMark(
        int column,
        CheckboxMultiMarkModelDecorator::UpdateCheckStateHandler updateHandler,
        CheckboxMultiMarkModelDecorator::IsCheckedHandler checkHandler)
    {
        return embeddNewModel(new CheckboxMultiMarkModelDecorator(column, updateHandler, checkHandler, m_parent));
    }

    template <class T>
    ModelBuilderCommon &withCheckboxMultiMark(int column, std::shared_ptr<ObservableSet<T>> list)
    {
        auto updateHandler = [list](const QVariant &item, bool state) -> void {
            auto typedItem = item.value<T>();
            if (state)
                list->insert(typedItem);
            else
                list->remove(typedItem);
        };

        auto checkHandler = [list](const QVariant &item) -> bool {
            auto typedItem = item.value<T>();
            return list->contains(typedItem);
        };

        return embeddNewModel(new CheckboxMultiMarkModelDecorator(column, updateHandler, checkHandler, m_parent));
    }

    QAbstractProxyModel *build() {
        auto wrapper = new SourceMappingModelDecorator(m_parent);
        wrapper->setSourceModel(m_base);
        return wrapper;
    }

private:
    ModelBuilderCommon &embeddNewModel(QAbstractProxyModel *model)
    {
        model->setSourceModel(m_base);
        m_base = model;
        return *this;
    }

    QObject * const m_parent;
    QAbstractItemModel *m_base;
};

class ColumnModelBuilder {
public:
    typedef std::function<QIdentityProxyModel*(int)> ModelFactory;

    template <class T>
    static ColumnModelBuilder AProperty(const QString &propertyName, QObject *parent = nullptr) {
        ColumnModelBuilder tmp;
        tmp.m_parent = parent;
        tmp.m_factories.append([propertyName, parent](int column) {
            return ObjectPropertyColumnModelDecorator::ofType<T>(column, propertyName, parent);
        });
        return tmp;
    }

    static ColumnModelBuilder ACustom(QObject *parent = nullptr) {
        ColumnModelBuilder tmp;
        tmp.m_parent = parent;
        return tmp;
    }

    ColumnModelBuilder &withHeader(const QString &headerText, QIcon icon = QIcon(), Qt::Orientation orientation = Qt::Horizontal) {
        auto parent = this->getParent();
        m_factories.append([headerText, orientation, icon, parent](int column) {
            auto func = [headerText, icon](int, int role, std::function<QVariant()> rawDataGetter) -> QVariant {
                return
                    role == Qt::DisplayRole ? headerText :
                    role == Qt::DecorationRole ? icon :
                    rawDataGetter();
            };
            return new SectionBasedHeaderDataModelDecorator(column, orientation, func, parent);
        });
        return *this;
    }

    ColumnModelBuilder &withMapData(int role, std::function<QVariant(const QModelIndex &index, const QVariant &rawValue)> mapHandler) {
        auto parent = this->getParent();
        m_factories.append([parent, role, mapHandler](int column) {
            auto func = [mapHandler](const QModelIndex &index, int, std::function<QVariant()> rawFunc) -> QVariant {
                return mapHandler(index, rawFunc());
            };
            return new ColumnAndRoleBasedDataModelDecorator(column, role, func, parent);
        });
        return *this;
    }

    ColumnModelBuilder &withData(int role, std::function<QVariant(const QModelIndex &index)> mapHandler) {
        auto parent = this->getParent();
        m_factories.append([parent, role, mapHandler](int column) {
            auto func = [mapHandler](const QModelIndex &index, int) -> QVariant {
                return mapHandler(index);
            };
            return new ColumnAndRoleBasedDataModelDecorator(column, role, func, parent);
        });
        return *this;
    }

    ColumnModelBuilder &withConstData(int role, std::function<QVariant()> mapHandler) {
        auto parent = this->getParent();
        m_factories.append([parent, role, mapHandler](int column) {
            auto func = [mapHandler](const QModelIndex &, int) -> QVariant {
                return mapHandler();
            };
            return new ColumnAndRoleBasedDataModelDecorator(column, role, func, parent);
        });
        return *this;
    }

    ColumnModelBuilder &withConstData(int role, const QVariant &value) {
        auto parent = this->getParent();
        m_factories.append([parent, role, value](int column) {
            auto func = [value](const QModelIndex &, int) -> QVariant {
                return value;
            };
            return new ColumnAndRoleBasedDataModelDecorator(column, role, func, parent);
        });
        return *this;
    }

    template <class T>
    ColumnModelBuilder &withModel() {
        auto parent = this->getParent();
        m_factories.append([parent](int column) {
            return new T(column, parent);
        });
        return *this;
    }

    ColumnModelBuilder &withData(std::function<QVariant(const QModelIndex &index, int role, std::function<QVariant()> rawValueGetter)> mapHandler) {
        auto parent = this->getParent();
        m_factories.append([parent, mapHandler](int column) {
            auto func = [mapHandler](const QModelIndex &index, int role, std::function<QVariant()> rawValueGetter) -> QVariant {
                return mapHandler(index, role, rawValueGetter);
            };
            return new ColumnBasedDataModelDecorator(column, func, parent);
        });
        return *this;
    }

    QAbstractItemModel *build(int column, QAbstractItemModel *startModel = nullptr) const {
        QAbstractItemModel *last = startModel;
        for (auto factory : m_factories) {
            auto model = factory(column);
            if (last != nullptr)
                model->setSourceModel(last);
            last = model;
        }
        return last;
    }

    QObject *getParent() const { return m_parent; }

private:
    QList<ModelFactory> m_factories;
    QObject *m_parent;
};

#include <qsharedpointer.h>
#include <qmetaobject.h>

class ColumnBuilderCollection
{
public:
    typedef QSharedPointer<ColumnBuilderCollection> Ptr;

    // Add filtering properties out
    // Apply model automatically base on the properties type
    template <class T, class TListType>
    static ColumnBuilderCollection::Ptr from(QObject *parent = nullptr, std::function<bool(const QMetaProperty &prop)> filter = nullptr) {
        if (filter == nullptr)
            filter = [](const QMetaProperty &prop) {
            return prop.type() != QVariant::ByteArray;
        };

        ColumnBuilderCollection::Ptr result(new ColumnBuilderCollection());

        QMetaObject metaObject = T::staticMetaObject;
        for (int i = 0; i < metaObject.propertyCount(); i++) {
            QMetaProperty metaProperty = metaObject.property(i);

            if (!filter(metaProperty))
                continue;

            QString propertyName = QString("%1").arg(metaProperty.name());

            auto builder = ColumnModelBuilder::AProperty<TListType>(propertyName, parent)
                .withHeader(propertyName);

            result->addProperty(propertyName, builder);
        }

        return result;
    }

    template <class T, class TListType>
    static ColumnBuilderCollection::Ptr from(QList<QString> properties, QObject *parent = nullptr) {
        ColumnBuilderCollection::Ptr result(new ColumnBuilderCollection());

        for (int i = 0; i < properties.length(); i++) {
            auto propertyName = properties[i];
            auto builder = ColumnModelBuilder::AProperty<TListType>(propertyName, parent)
                .withHeader(propertyName);
            result->addProperty(propertyName, builder);
        }

        return result;
    }

    void addColumn(const ColumnModelBuilder &column) {
        m_builders.append(column);
    }

    void addProperty(const QString &propertyName, const ColumnModelBuilder &column) {
        m_indexMap[propertyName] = m_builders.size();
        m_builders.append(column);
    }

    ColumnModelBuilder &getProperty(const QString &propertyName) {
        return m_builders[m_indexMap[propertyName]];
    }

    const QList<ColumnModelBuilder> &toList() const {
        return m_builders;
    }

private:
    QHash<QString, int> m_indexMap;
    QList<ColumnModelBuilder> m_builders;
};

class ModelBuilderOf final
{
public:
    ModelBuilderOf(const QObservableVariantListProxy &list, QObject *parent)
        : m_parent(parent)
    {
        auto model = new ObservableVariantListModel(parent);
        model->setSource(list);
        m_base = model;
    }

    ModelBuilderCommon withColumns(int columns)
    {
        auto model = new ColumnModelDecorator(columns, m_parent);
        model->setSourceModel(m_base);
        return ModelBuilderCommon(model, m_parent);
    }

    ModelBuilderCommon withColumns(const QList<ColumnModelBuilder> columnModelBuilders)
    {
        auto model = new ColumnModelDecorator(columnModelBuilders.size(), m_parent);
        model->setSourceModel(m_base);

        QAbstractItemModel *last = model;
        for (int i = 0; i < columnModelBuilders.size(); i++) {
            const auto &columnBuilder = columnModelBuilders[i];
            last = columnBuilder.build(i, last);
        }

        return ModelBuilderCommon(last, m_parent);
    }

private:
    QObject * const m_parent;
    QAbstractItemModel *m_base;
};


class ModelBuilder final
{
public:
    static ModelBuilderOf AModelFor(const QObservableVariantListProxy &list, QObject *parent = nullptr)
    {
        return ModelBuilderOf(list, parent);
    }

    template <class T>
    static ModelBuilderOf AModelFor(QObservableList<T> &list, QObject *parent = nullptr)
    {
        return ModelBuilderOf(QObservableVariantListProxy::from(list), parent);
    }

    static ModelBuilderCommon AListModelFor(const QObservableVariantListProxy &list, QObject *parent = nullptr)
    {
        return ModelBuilderOf(list, parent)
            .withColumns(1);
    }

    template <class T>
    static ModelBuilderCommon AListModelFor(QObservableList<T> &list, QObject *parent = nullptr)
    {
        return ModelBuilderOf(QObservableVariantListProxy::from(list), parent)
            .withColumns(1);
    }

    static ModelBuilder AModel(QObject *parent = nullptr)
    {
        return ModelBuilder(parent);
    }

    ModelBuilderOf of(const QObservableVariantListProxy &list)
    {
        return ModelBuilderOf(list, m_parent);
    }

    template <class T>
    ModelBuilderOf of(QObservableList<T> &list)
    {
        return of(QObservableVariantListProxy::from(list));
    }

private:
    ModelBuilder(QObject *parent)
        : m_parent(parent)
    {}
    QObject * const m_parent;
};
