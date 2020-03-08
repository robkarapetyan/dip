#include "port.h"
#include <QPainter>
#include <QDebug>

Port::Port()
{
    this->setFlag(ItemIsMovable);
    this->setFiltersChildEvents(true);
//    this->m_pin.setFlag(ItemIsMovable);
    m_pin->setParentItem(this);
//    m_pin->installSceneEventFilter(this);
    //m_pin.setRect(0,0,6,6);
//    this->scenep
}

Port::~Port(){}

void Port::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{
//    painter->setBrush(QBrush(Qt::green));
//    painter->setPen(Qt::black);
//    painter->drawRect(this->boundingRect().x(),this->boundingRect().y(),this->boundingRect().width() + 10, this->boundingRect().height() + 10);
}

QRectF Port::boundingRect() const
{
    return this->childrenBoundingRect();
}

void Port::setrect(qreal ax, qreal ay, qreal w, qreal h)
{
    m_pin->setRect(ax,ay,w,h);
}

QPointF Port::scenePos() const
{
    return m_pin->scenePos();
}

void Port::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << event->scenePos();
}
