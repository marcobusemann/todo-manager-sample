#pragma once

//#include "../observableset.h"

#include <QItemSelectionModel>
#include <QAbstractItemView>

#include <memory>

template <class T>
class SelectionManager
{
public:
    typedef std::shared_ptr<SelectionManager<T>> Ptr;

    static Ptr from(QAbstractItemView *view)
    {
        return std::make_shared<SelectionManager<T>>(view);
    }

public:
    SelectionManager(QAbstractItemView *view)
        : m_viewSelectionModel(view->selectionModel())
    {
        Q_ASSERT(m_viewSelectionModel);
        view->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    }

    void setObservableMultiMarkItems(const std::shared_ptr<ObservableSet<T>> &items)
    {
        //m_multiMarkItems = items;
    }

    QSet<T> getSelectedItems(bool considerMultiMark) const
    {
		/*
        QSet<T> result;
        if (considerMultiMark && m_multiMarkItems)
            result = m_multiMarkItems->toSet();
        else
        {
            auto indexes = m_viewSelectionModel->selectedRows();
            foreach(QModelIndex idx, indexes)
                result.insert(idx.data(Qt::UserRole).value<T>());
        }
        return result;
		*/
    }

private:
    //std::shared_ptr<ObservableSet<T>> m_multiMarkItems;
    QItemSelectionModel *m_viewSelectionModel;
};
