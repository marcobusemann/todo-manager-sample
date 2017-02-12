#pragma once

#include <QMainWindow>

#include <QMetaOrm/SessionFactory.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(
            const QMetaOrm::SessionFactory::Ptr &sessionFactory, QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

