#include "iline.h"

ILine::ILine(Pin* a, Pin* b):start(a), end(b)
{

}

QRectF ILine::boundingRect() const
{
    return QRectF();
}

void ILine::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{}


ILine::~ILine()
{

}
