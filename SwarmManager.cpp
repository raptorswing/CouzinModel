#include "SwarmManager.h"

#include <QtDebug>
#include <QDateTime>
#include <QVector2D>
#include <cmath>
#include <QThreadPool>

#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

#include "SwarmAgentItem.h"
#include "BuildNeighborsRunnable.h"
#include "CalcNewPositionRunnable.h"

SwarmManager::SwarmManager(QGraphicsScene * scene,
                           QObject *parent) :
    QObject(parent), scene(scene)
{
    qsrand(QDateTime::currentDateTime().toTime_t());

    this->iterateTimer = new QTimer(this);
    connect(this->iterateTimer,
            SIGNAL(timeout()),
            this,
            SLOT(iterateSwarm()));
    this->iterateTimer->start(10);

    this->currentSettings.zoneOfRepulsion = 0.0;
    this->currentSettings.zoneOfAttraction = 0.0;
    this->currentSettings.zoneOfOrientation = 0.0;
}

void SwarmManager::resetSwarm(int numAgents)
{
    qDebug() << "Reset swarm with" << numAgents << "agents";
    if (this->scene.isNull())
        return;

    this->scene->clear();
    this->agents.clear();

    for (int i = 0; i < numAgents; i++)
    {
        SwarmAgentItem * item = new SwarmAgentItem();
        this->scene->addItem(item);
        item->setPos(qrand() % 20,
                     qrand() % 20);
        agents.append(item);
    }
}

void SwarmManager::adjustSwarmSettings(SwarmSettingsWidget::SwarmSettings s)
{
    qDebug() << "New swarm settings:" << s.zoneOfRepulsion << s.zoneOfAttraction << s.zoneOfOrientation;
    this->currentSettings = s;

}

void SwarmManager::iterateSwarm()
{
    if (this->agents.size() == 0)
        return;

    QThreadPool * pool = QThreadPool::globalInstance();

    //First, calculate neighbors
    foreach(SwarmAgentItem * agent, this->agents)
    {
        BuildNeighborsRunnable * run = new BuildNeighborsRunnable(agent,&this->agents,&this->currentSettings);
        pool->start(run);
    }
    pool->waitForDone();

    //Build laplacian
    /*
    const int n = this->agents.size();
    Eigen::MatrixXd laplacian(n,n);
    for (int i = 0; i < n; i++)
    {
        SwarmAgentItem * me = this->agents[i];

        //QString rowString;
        for (int j = 0; j < n; j++)
        {
            SwarmAgentItem * them = this->agents[j];

            if (i == j)
                laplacian(i,j) = me->neighbors.size();
            else if (me->neighbors.contains(them))
            {
                laplacian(i,j) = -1;
                laplacian(j,i) = -1;
            }
            else if (laplacian(j,i) == -1)
                laplacian(i,j) = -1;
            else
            {
                laplacian(i,j) = 0;
                laplacian(j,i) = 0;
            }

            //rowString.append(QString::number(laplacian(i,j)).rightJustified(3) + " ");
        }
        //qDebug() << rowString;
    }
    //qDebug() << "---";

    for (int i = 0; i < n; i++)
    {
        int connCount = 0;
        for (int j = 0; j < n; j++)
        {
            if (laplacian(i,j) == -1)
                connCount++;
        }
        laplacian(i,i) = connCount;
    }


    Eigen::EigenSolver<Eigen::MatrixXd> eigenSolver;
    eigenSolver.compute(laplacian,false);
    Eigen::Matrix<std::complex<double>,-1,1> results = eigenSolver.eigenvalues();

    QList<qreal> eigenvalues;
    for (int i = 0; i < results.rows(); i++)
        eigenvalues.append(results[i].real());
    qSort(eigenvalues);
    if (eigenvalues.size() > 2)
        qDebug() << eigenvalues[1];
    */


    //Calculate new directions of travel
    foreach(SwarmAgentItem * agent, this->agents)
    {
        CalcNewPositionRunnable * runner = new CalcNewPositionRunnable(agent,&this->agents,&this->currentSettings);
        pool->start(runner);
    }
    pool->waitForDone();

    //Make changes
    foreach(SwarmAgentItem * agent, this->agents)
    {
        qreal desired = agent->getDesiredDirection();
        qreal current = agent->getCurrentDirection();

        if (qAbs<qreal>(desired - current) > 180.0)
        {
            if (desired > current)
                desired -= 360.0;
            else
                current -= 360.0;
        }

        if (qAbs<qreal>(desired - current) <= this->currentSettings.turningRateDegrees)
            current = desired;
        else if (current < desired)
            current += this->currentSettings.turningRateDegrees;
        else
            current -= this->currentSettings.turningRateDegrees;

        agent->setCurrentDirection(current + (qrand() % 9)-4);
        //agent->setCurrentDirection(current);

        QVector2D movement(cos(current*3.14159265/180.0),sin(current*3.14159265/180.0));
        movement.normalize();

        QPointF currentPos = agent->pos();
        movement *= this->currentSettings.speed;
        currentPos.setX(currentPos.x() + movement.x());
        currentPos.setY(currentPos.y() + movement.y());
        agent->setPos(currentPos);
    }
}
