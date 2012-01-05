#ifndef SWARMMANAGER_H
#define SWARMMANAGER_H

#include <QObject>

#include <QGraphicsScene>
#include <QPointer>
#include <QTimer>
#include <QList>

#include "SwarmSettingsWidget.h"

class SwarmAgentItem;

class SwarmManager : public QObject
{
    Q_OBJECT
public:
    explicit SwarmManager(QGraphicsScene * scene,
                          QObject *parent = 0);

signals:

public slots:
    void resetSwarm(int numAgents);
    void adjustSwarmSettings(SwarmSettingsWidget::SwarmSettings);
    void iterateSwarm();


private:
    QPointer<QGraphicsScene> scene;

    QTimer * iterateTimer;

    QList<SwarmAgentItem *> agents;

    SwarmSettingsWidget::SwarmSettings currentSettings;

};

#endif // SWARMMANAGER_H
