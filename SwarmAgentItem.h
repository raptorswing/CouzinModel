#ifndef SWARMAGENTITEM_H
#define SWARMAGENTITEM_H

#include <QGraphicsItem>

#include <QSet>

#include "SwarmSettingsWidget.h"

class SwarmAgentItem : public QGraphicsItem
{
public:
    explicit SwarmAgentItem(QGraphicsItem *parent = 0);

    //pure-virtual from QGraphicsItem
    QRectF boundingRect() const;

    //pure-virtual from QGraphicsItem
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    qreal getCurrentDirection() const;
    qreal getCurrentDirectionRadians() const;
    qreal getDesiredDirection() const;
    qreal getDesiredDirectionRadians() const;
    //qreal getCurrentSpeed() const;

    void setCurrentDirection(qreal d);
    void setCurrentDirectionRadians(qreal d);
    void setDesiredDirection(qreal d);
    void setDesiredDirectionRadians(qreal d);
    //void setCurrentSpeed(qreal s);

    QSet<SwarmAgentItem *> neighbors;

    void buildNeighborsCouzin(const QList<SwarmAgentItem *>* agents,
                              const SwarmSettingsWidget::SwarmSettings * settings);

    void buildNeighborsNearest(const QList<SwarmAgentItem *>* agents,
                               const SwarmSettingsWidget::SwarmSettings * settings);

    void calcNewPositionCouzin(const QList<SwarmAgentItem *> * agents,
                               const SwarmSettingsWidget::SwarmSettings * settings);

signals:

public slots:

private:
    qreal currentDirection;
    qreal desiredDirection;
    qreal currentSpeed;

};

#endif // SWARMAGENTITEM_H
