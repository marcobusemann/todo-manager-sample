#pragma once

#include <QScopedPointer>
#include <QSharedPointer>
#include <QMainWindow>

#include <QMetaOrm/SessionFactory.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(
        const QMetaOrm::SessionFactory::Ptr &sessionFactory, 
        QWidget *parent = 0);
    ~MainWindow();

private:
    QScopedPointer<Ui::MainWindow> m_ui;
};

