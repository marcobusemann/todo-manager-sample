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
class SourceMappingModelDecorator : public QIdentityProxyModel
{
public: 
   SourceMappingModelDecorator(QObject *parent)
      : QIdentityProxyModel(parent)
   {}

    virtual QModelIndex	mapFromRoot(const QModelIndex &rootIndex) const
    {
       const QAbstractProxyModel *origin = this;
       QModelIndex result = mapFromSource(rootIndex);

       while ((origin = dynamic_cast<QAbstractProxyModel*>(origin->sourceModel())) != nullptr)
          result = origin->mapFromSource(result);

       return result;
    }

    virtual QModelIndex	mapToRoot(const QModelIndex &proxyIndex) const
    {
       const QAbstractProxyModel *origin = this;
       QModelIndex result = mapToSource(proxyIndex);

       while ((origin = dynamic_cast<QAbstractProxyModel*>(origin->sourceModel())) != nullptr)
          result = origin->mapToSource(result);

       return result;
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

	SourceMappingModelDecorator *build() {
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
