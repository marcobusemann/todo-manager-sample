#include "checkboxmultimarkviewmanager.h"

//-----------------------------------------------------------------------------
CheckboxMultiMarkViewManager::Ptr CheckboxMultiMarkViewManager::from(int column, QHeaderView *view)
{
    return std::make_shared<CheckboxMultiMarkViewManager>(column, view);
}

//-----------------------------------------------------------------------------
CheckboxMultiMarkViewManager::CheckboxMultiMarkViewManager(int column, QHeaderView *view)
    : m_enabled(true)
    , m_column(column)
    , m_view(view)
{
}

//-----------------------------------------------------------------------------
void CheckboxMultiMarkViewManager::setEnabled(bool isEnabled)
{
    if (m_enabled != isEnabled)
    {
        m_enabled = isEnabled;
        updateView();
        if (OnStateChanged)
            OnStateChanged(isEnabled);
    }
}

//-----------------------------------------------------------------------------
bool CheckboxMultiMarkViewManager::isEnabled() const
{
    return m_enabled;
}

//-----------------------------------------------------------------------------
void CheckboxMultiMarkViewManager::toggleEnabled()
{
    setEnabled(!isEnabled());
}

//-----------------------------------------------------------------------------
void CheckboxMultiMarkViewManager::updateView()
{
    m_view->setSectionHidden(m_column, !m_enabled);
}