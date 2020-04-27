#include "ortogonallineitem.h"

OrtogonalLineItem::OrtogonalLineItem()
{
    setAcceptHoverEvents(true);
    setFlag(ItemSendsScenePositionChanges);
    setFlag(ItemIsMovable);
//    setFlag(ItemIgnoresParentOpacity);
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

//        qDebug() << this;

}

void OrtogonalLineItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    setPen(QPen());
}

QVariant OrtogonalLineItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == QGraphicsItem::ItemPositionChange){
        if(orient == Qt::Orientation::Horizontal){
            if(previous_sibling){
//                qDebug() << value.toPointF().y() << "this scenepos";
//                qDebug() << previous_sibling->line().p2().ry()  << "this sceneposssssss";
//                qDebug() << pos().y() << "kkkk";

//                this->previous_sibling->setLine(QLineF(previous_sibling->line().p1(),
//                                                       QPointF( previous_sibling->line().p2().rx() + pos().x(),
//                                                                previous_sibling->line().p2().ry() + ( value.toPointF().y() - pos().y() >= 0 ? 1 : -1)
//                                                                )));

//                qDebug() << mapToScene(this->pos()) << "mapped to parent";
//                qDebug() << this->mapFromScene(this->pos()) << "mapped from parent";

//                        previous_sibling->line().setLength(previous_sibling->line().length() + )
                                //setLine(mapToItem(this->parentItem(), 0, 0).rx(),mapToItem(this->parentItem(), 0, 0).ry(), 200, 200);
//                                          this->mapToItem(this->parentItem(), pos()).x(), this->mapToItem(this->parentItem(), value.toPointF()).y());
            }
            return QPointF(pos().x(), value.toPointF().y());
        }
        else{

            return QPointF(value.toPointF().x(), pos().y());
        }
    }
    return QGraphicsLineItem::itemChange(change,value);

}

void OrtogonalLineItem::set_siblings(OrtogonalLineItem *prev, OrtogonalLineItem *next)
{
    previous_sibling = prev;
    next_sibling = next;
}

void OrtogonalLineItem::setOrientation(const Qt::Orientation &a)
{
    orient = a;
}
