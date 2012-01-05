#ifndef CALCNEWPOSITIONRUNNABLE_H
#define CALCNEWPOSITIONRUNNABLE_H

#include <QRunnable>
#include <QList>

#include "SwarmSettingsWidget.h"

class SwarmAgentItem;

class CalcNewPositionRunnable : public QRunnable
{
public:
    CalcNewPositionRunnable(SwarmAgentItem * agent,
                            const QList<SwarmAgentItem *> * agents,
                            const SwarmSettingsWidget::SwarmSettings * settings);

    void run();

private:
    SwarmAgentItem * agent;
    const QList<SwarmAgentItem *> * agents;
    const SwarmSettingsWidget::SwarmSettings * settings;
};

#endif // CALCNEWPOSITIONRUNNABLE_H
