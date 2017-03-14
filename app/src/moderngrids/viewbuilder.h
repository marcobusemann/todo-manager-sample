#pragma once

#include "managers/checkboxmultimarkviewmanager.h"
#include "managers/selectionmanager.h"

template <class TView, class TObjectType>
class ViewComposition
{
public:
    TView *m_view;
    CheckboxMultiMarkViewManager::Ptr m_multiMarkManager;
    SelectionManager<TObjectType>::Ptr m_selectionManager;
};

template <class TObjectType>
class ItemViewBuilderCommon final
{
public:
    ItemViewBuilderCommon(QAbstractItemModel *model, QWidget *parent)
        : m_parent(parent)
        , m_model(model)
        , m_checkboxMultiMarkColumn(-1)
    {
    }

    ItemViewBuilderCommon withCheckboxMultiMark(int column, ObservableSet<Person*>::Ptr items)
    {
        m_checkboxMultiMarkColumn = column;
        m_checkboxMultiMarkItems = items;
    }

    ViewComposition<QTableView, TObjectType> buildTable()
    {
        ViewComposition<QTableView> composition;
        composition.m_view = new QTableView(m_parent);
        composition.m_view->setModel(m_model);
        composition.m_selectionManager = SelectionManager<TObjectType>::from(composition.m_view);

        buildCheckboxMultiMarkViewManager<QTableView>(composition);

        return composition;
    }

private:
    template <class T>
    void buildCheckboxMultiMarkViewManager(ViewComposition<T, TObjectType> &composition)
    {
        if (m_checkboxMultiMarkColumn >= 0)
        {
            composition.m_multiMarkManager = CheckboxMultiMarkViewManager::from(m_checkboxMultiMarkColumn, composition.m_view->horizontalHeader());
            composition.m_multiMarkManager->OnStateChanged = [m_checkboxMultiMarkItems](bool isEnabled) -> void {
                if (isEnabled == false)
                    m_checkboxMultiMarkItems->clear();
            };
            composition.m_selectionManager->setObservableMultiMarkItems(m_checkboxMultiMarkItems);
        }
    }

private:
    QWidget * const m_parent;
    QAbstractItemModel *m_model;

    int m_checkboxMultiMarkColumn;
    ObservableSet<TObjectType>::Ptr m_checkboxMultiMarkItems;
};

template <class TObjectType>
class ItemViewBuilder final
{
public:
    static ItemViewBuilder<TObjectType> AView(QWidget *parent = nullptr)
    {
        return ItemViewBuilder(parent);
    }

    ItemViewBuilderCommon<TObjectType> of(QAbstractItemModel *model)
    {
        return ItemViewBuilderCommon(model, m_parent);
    }

private:
    ItemViewBuilder(QWidget *parent)
        : m_parent(parent)
    {}
    QWidget * const m_parent;
};