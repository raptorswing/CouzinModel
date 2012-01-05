#include "BuildNeighborsRunnable.h"

#include "SwarmAgentItem.h"

BuildNeighborsRunnable::BuildNeighborsRunnable(SwarmAgentItem * agent,
                                               const QList<SwarmAgentItem *> * agents,
                                               const SwarmSettingsWidget::SwarmSettings * settings) :
    agent(agent), agents(agents), settings(settings)
{
    this->setAutoDelete(true);
}

void BuildNeighborsRunnable::run()
{


    this->agent->buildNeighborsCouzin(this->agents,
                                      this->settings);


    /*
    this->agent->buildNeighborsNearest(this->agents,
                                       this->settings);
                                      */

}
