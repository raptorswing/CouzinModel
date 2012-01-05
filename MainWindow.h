#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>

#include <QGraphicsView>
#include <QGraphicsScene>

#include "SwarmSettingsWidget.h"
#include "SwarmManager.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionSwarm_Settings_triggered();

    void resetSceneRect();

private:
    Ui::MainWindow *ui;

    QGraphicsScene * scene;
    QGraphicsView * view;

    QPointer<SwarmSettingsWidget> swarmSettingsWidget;
    SwarmManager * swarmManager;
};

#endif // MAINWINDOW_H
