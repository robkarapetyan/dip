#include "ortogonallineitem.h"

OrtogonalLineItem::OrtogonalLineItem()
{
    setAcceptHoverEvents(true);
    setFlag(ItemSendsScenePositionChanges);
    setFlag(ItemIsMovable);
}

OrtogonalLineItem::~OrtogonalLineItem()
{

}

void OrtogonalLineItem::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    setPen(QPen(QBrush(Qt::gray), 3, Qt::PenStyle::DashDotLine, Qt::SquareCap, Qt::BevelJoin));

    if(orient == Qt::Orientation::Vertical)
    this->setCursor(QCursor(QPixmap("://icons/moveh.png")));
    else
    this->setCursor(QCursor(QPixmap("://icons/movev.png")));

}

void OrtogonalLineItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    setPen(QPen());
}

QVariant OrtogonalLineItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == QGraphicsItem::ItemPositionChange){
        if(orient == Qt::Orientation::Horizontal)
        return QPointF(pos().x(), value.toPointF().y());
        else
        return QPointF(value.toPointF().x(), pos().y());
    }
    return QGraphicsLineItem::itemChange(change,value);

}

void OrtogonalLineItem::setOrientation(const Qt::Orientation &a)
{
    orient = a;
}
