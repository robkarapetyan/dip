#include "pin.h"
#include <QBrush>

Pin::Pin()
{
    this->setAcceptHoverEvents(true);
}

Pin::~Pin()
{

}

void Pin::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug("pin click");
    QGraphicsRectItem::mousePressEvent(event);
}

void Pin::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug("pin hovered");
    this->setBrush(QBrush(Qt::red));
    QGraphicsRectItem::hoverEnterEvent(event);
}

void Pin::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug("pin unhovered");
    this->setBrush(QBrush(Qt::black));
    QGraphicsRectItem::hoverLeaveEvent(event);
}
