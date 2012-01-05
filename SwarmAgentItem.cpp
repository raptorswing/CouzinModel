#include "SwarmAgentItem.h"

#include <QPainter>
#include <QtDebug>
#include <QVector2D>
#include <cmath>

SwarmAgentItem::SwarmAgentItem(QGraphicsItem *parent) :
    QGraphicsItem(parent), currentDirection(0.0), desiredDirection(0.0)
{
    this->currentDirection = qrand() % 360;
    this->desiredDirection = currentDirection;

    this->setZValue(10.0);
}

//pure-virtual from QGraphicsItem
QRectF SwarmAgentItem::boundingRect() const
{
    //return QRectF(-2,-2,4.0,4.0);
    return QRectF(-1,-1,2,2);
}

//pure-virtual from QGraphicsItem
void SwarmAgentItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->rotate(90);
    painter->setRenderHint(QPainter::Antialiasing,true);
    QRectF rect = this->boundingRect();
    painter->drawLine(rect.bottomLeft(),QPointF(rect.center().x(),
                                                rect.top()));
    painter->drawLine(rect.bottomRight(),QPointF(rect.center().x(),
                                                rect.top()));
}

qreal SwarmAgentItem::getCurrentDirection() const
{
    return this->currentDirection;
}

qreal SwarmAgentItem::getCurrentDirectionRadians() const
{
    return this->getCurrentDirection()*3.14159265/180.0;
}

qreal SwarmAgentItem::getDesiredDirection() const
{
    return this->desiredDirection;
}

qreal SwarmAgentItem::getDesiredDirectionRadians() const
{
    return this->getDesiredDirection()*3.14159265/180.0;
}

/*
qreal SwarmAgentItem::getCurrentSpeed() const
{
    return this->currentSpeed;
}
*/

void SwarmAgentItem::setCurrentDirection(qreal d)
{
    while (d >= 360.0)
        d -= 360.0;
    while (d < 0.0)
        d += 360.0;
    this->currentDirection = d;
    this->setRotation(d);
}

void SwarmAgentItem::setCurrentDirectionRadians(qreal d)
{
    this->setCurrentDirection(d*180.0/3.14159265);
}

void SwarmAgentItem::setDesiredDirection(qreal d)
{
    while (d >= 360.0)
        d -= 360.0;
    while (d < 0.0)
        d += 360.0;
    this->desiredDirection = d;
}

void SwarmAgentItem::setDesiredDirectionRadians(qreal d)
{
    this->setDesiredDirection(d*180.0/3.14159265);
}

void SwarmAgentItem::buildNeighborsCouzin(const QList<SwarmAgentItem *>* agents,
                                          const SwarmSettingsWidget::SwarmSettings * settings)
{
    this->neighbors.clear();

    bool anyInRepulsionZone = false;
    for (int i = 0; i < agents->size(); i++)
    {
        SwarmAgentItem * agent = agents->at(i);
        if (agent == this)
            continue;

        QVector2D vec((agent->pos() - this->pos()));

        if (vec.length() < settings->zoneOfRepulsion)
        {
            if (!anyInRepulsionZone)
            {
                anyInRepulsionZone = true;
                this->neighbors.clear();
            }
            this->neighbors.insert(agent);
        }

        //Don't bother adding attractive/orientative neighbors if we've got any repulsion
        if (anyInRepulsionZone)
            continue;

        QVector2D myDirectionVector(cos(this->getCurrentDirectionRadians()),
                                    sin(this->getCurrentDirectionRadians()));
        myDirectionVector.normalize();
        qreal dot = QVector2D::dotProduct(vec,myDirectionVector);
        qreal angle = acos(dot/(vec.length()*myDirectionVector.length())) * 180.0/3.14159265;

        if (angle < settings->alpha/2 && (vec.length() < settings->zoneOfAttraction || vec.length() < settings->zoneOfOrientation))
            this->neighbors.insert(agent);
    }
}

void SwarmAgentItem::buildNeighborsNearest(const QList<SwarmAgentItem *> *agents,
                                           const SwarmSettingsWidget::SwarmSettings *settings)
{
    this->buildNeighborsCouzin(agents,settings);

    if (this->neighbors.isEmpty())
        return;

    const int max = 5;

    QList<qreal> distances;
    QSetIterator<SwarmAgentItem *> iter(this->neighbors);
    while (iter.hasNext())
    {
        const SwarmAgentItem * item = iter.next();
        if (item == this)
            continue;
        const QVector2D vec(item->pos() - this->pos());

        distances.append(vec.length());
    }

    qSort(distances);

    qreal toBeat = distances[qMin(max-1,distances.size()-1)];

    QMutableSetIterator<SwarmAgentItem *> iterMut(this->neighbors);
    while (iterMut.hasNext())
    {
        const SwarmAgentItem * item = iterMut.next();
        if (item == this)
            continue;
        const QVector2D vec(item->pos() - this->pos());
        if (vec.length() > toBeat)
            iterMut.remove();
    }
}

void SwarmAgentItem::calcNewPositionCouzin(const QList<SwarmAgentItem *> *agents,
                                           const SwarmSettingsWidget::SwarmSettings *settings)
{
    bool anyInRepulsionZone = false;

    //Process repulsion
    QVector2D repulse(0,0);
    QSetIterator<SwarmAgentItem *> iter(this->neighbors);
    while (iter.hasNext())
    {
        SwarmAgentItem * agent = iter.next();
        if (agent == this)
            continue;

        const QVector2D vec((agent->pos() - this->pos()));

        if (vec.length() < settings->zoneOfRepulsion)
        {
            if (!anyInRepulsionZone)
                anyInRepulsionZone = true;
            repulse += vec.normalized();
        }
    }
    repulse = -1*repulse.normalized();

    if (anyInRepulsionZone)
    {
        this->setDesiredDirectionRadians(atan2(repulse.y(),repulse.x()));
        return;
    }

    //If we didn't have any repulsive stuff to process, do attraction and orientation
    bool anyInAttraction = false;
    bool anyInOrientation = false;

    QVector2D attract(0,0);
    QVector2D orient(0,0);

    iter = QSetIterator<SwarmAgentItem *>(this->neighbors);
    while (iter.hasNext())
    {
        SwarmAgentItem * agent = iter.next();
        if (agent == this)
            continue;

        const QVector2D vec((agent->pos() - this->pos()));
        if (vec.length() < settings->zoneOfAttraction)
        {
            if (!anyInAttraction)
                anyInAttraction = true;
            if (agent != agents->at(0) && agent != agents->at(1))
                attract += vec.normalized();
            else
                attract += QVector2D(1,0);
        }

        if (vec.length() < settings->zoneOfOrientation)
        {
            if (!anyInOrientation)
                anyInOrientation = true;
            const QVector2D theirDirectionVector(cos(agent->getCurrentDirectionRadians()),
                                                 sin(agent->getCurrentDirectionRadians()));
            if (agent != agents->at(0) && agent != agents->at(1))
                orient += theirDirectionVector.normalized();
            else
                orient += QVector2D(1,0);
        }
    }

    attract.normalize();
    orient.normalize();

    QVector2D result;
    if (anyInAttraction && anyInOrientation)
        result = (attract + orient).normalized();
    else if (anyInAttraction)
        result = attract;
    else if (anyInOrientation)
        result = orient;


    if (!result.isNull())
        this->setDesiredDirectionRadians(atan2(result.y(),result.x()));
}

/*
void SwarmAgentItem::setCurrentSpeed(qreal s)
{
    this->currentSpeed = s;
}
*/
