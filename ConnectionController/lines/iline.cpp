#include "iline.h"

ILine::ILine(Pin* a, Pin* b):start(a), end(b)
{

}

QRectF ILine::boundingRect() const
{
    if(!this->childItems().empty()){
        return childItems().at(0)->boundingRect();
    }
    qDebug() << "aaa";
    return childrenBoundingRect();
}

void ILine::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{}


ILine::~ILine()
{

}
