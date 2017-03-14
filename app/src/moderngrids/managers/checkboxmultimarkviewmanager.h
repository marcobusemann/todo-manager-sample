#pragma once

#include <QHeaderView>

#include <functional>
#include <memory>

class CheckboxMultiMarkViewManager
{
public:
    typedef std::function<void(bool)> StateChangedHandler;
    typedef std::shared_ptr<CheckboxMultiMarkViewManager> Ptr;

    static Ptr from(int column, QHeaderView *view);

    StateChangedHandler OnStateChanged;

public:
    CheckboxMultiMarkViewManager(int column, QHeaderView *view);
    void setEnabled(bool isEnabled);
    bool isEnabled() const;
    void toggleEnabled();

private:
    void updateView();

    bool m_enabled;
    int m_column;
    QHeaderView *m_view;
};
