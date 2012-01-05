#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->scene = new QGraphicsScene(this);
    this->swarmManager = new SwarmManager(this->scene,this);

    this->view = new QGraphicsView(this->scene,this);
    this->view->setDragMode(QGraphicsView::ScrollHandDrag);
    this->view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    this->view->scale(2.0,2.0);

    this->setCentralWidget(this->view);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSwarm_Settings_triggered()
{
    if (this->swarmSettingsWidget.isNull())
    {
        this->swarmSettingsWidget = new SwarmSettingsWidget();
        this->swarmSettingsWidget->setAttribute(Qt::WA_DeleteOnClose);

        SwarmSettingsWidget * raw = this->swarmSettingsWidget.data();

        connect(this,
                SIGNAL(destroyed()),
                raw,
                SLOT(deleteLater()));


        connect(raw,
                SIGNAL(restartSignalled(int)),
                this->swarmManager,
                SLOT(resetSwarm(int)));

        connect(raw,
                SIGNAL(restartSignalled(int)),
                this,
                SLOT(resetSceneRect()));

        connect(raw,
                SIGNAL(swarmSettingsChanged(SwarmSettingsWidget::SwarmSettings)),
                this->swarmManager,
                SLOT(adjustSwarmSettings(SwarmSettingsWidget::SwarmSettings)));

        this->swarmSettingsWidget->show();
        this->swarmSettingsWidget->raise();
    }
    else if (this->swarmSettingsWidget->isHidden())
    {
        this->swarmSettingsWidget->raise();
        this->swarmSettingsWidget->show();
    }
    else
    {
        this->swarmSettingsWidget->lower();
        this->swarmSettingsWidget->hide();
    }
}

void MainWindow::resetSceneRect()
{
    this->view->centerOn(0,0);
}
