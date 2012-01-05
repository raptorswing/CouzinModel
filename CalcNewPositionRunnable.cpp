#include "CalcNewPositionRunnable.h"

#include <SwarmAgentItem.h>

CalcNewPositionRunnable::CalcNewPositionRunnable(SwarmAgentItem * agent,
                                                 const QList<SwarmAgentItem *> * agents,
                                                 const SwarmSettingsWidget::SwarmSettings * settings) :
    agent(agent), agents(agents), settings(settings)
{
    this->setAutoDelete(true);
}

void CalcNewPositionRunnable::run()
{
    this->agent->calcNewPositionCouzin(this->agents,
                                       this->settings);
}
