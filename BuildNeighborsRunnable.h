#ifndef BUILDNEIGHBORSRUNNABLE_H
#define BUILDNEIGHBORSRUNNABLE_H

#include <QRunnable>
#include <QList>

#include "SwarmSettingsWidget.h"

class SwarmAgentItem;

class BuildNeighborsRunnable : public QRunnable
{
public:
    BuildNeighborsRunnable(SwarmAgentItem * agent,
                           const QList<SwarmAgentItem *> * agents,
                           const SwarmSettingsWidget::SwarmSettings * settings);

    void run();

private:
    SwarmAgentItem * agent;
    const QList<SwarmAgentItem *> * agents;
    const SwarmSettingsWidget::SwarmSettings * settings;
};

#endif // BUILDNEIGHBORSRUNNABLE_H
