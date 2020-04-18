#include "flatline.h"

FlatLine::FlatLine(Pin* a, Pin* b) : ILine(a, b)
{
    _line.setParentItem(this);
    _line.setAcceptHoverEvents(true);
    setHandlesChildEvents(true);
}

FlatLine::~FlatLine()
{

}

void FlatLine::setLine(qreal x1, qreal y1, qreal x2, qreal y2)
{
    this->_line.setLine(x1, y1, x2, y2);
}

void FlatLine::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu * menu  = new QMenu;
    menu->setAttribute(Qt::WA_DeleteOnClose, true);
    menu->addAction("Make Ortogonal");
    menu->addAction("Properties");
    menu->addAction("Delete");
    menu->exec(event->screenPos());
}

void FlatLine::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    _line.setPen(QPen(QBrush(Qt::gray), 3, Qt::PenStyle::DashDotLine, Qt::SquareCap, Qt::BevelJoin));
}

void FlatLine::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    _line.setPen(QPen());
}
