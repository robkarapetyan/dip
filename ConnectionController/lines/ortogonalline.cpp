#include "ortogonalline.h"

OrtogonalLine::OrtogonalLine(Pin* a, Pin* b) : ILine(a, b)
{
    setFiltersChildEvents(true);

    OrtogonalLineItem* ort1 = new OrtogonalLineItem;
    OrtogonalLineItem* ort2 = new OrtogonalLineItem;
    OrtogonalLineItem* ort3 = new OrtogonalLineItem;

    ort2->setOrientation(Qt::Vertical);

    ort1->setParentItem(this);
    ort2->setParentItem(this);
    ort3->setParentItem(this);
    vec_of_sublines.push_back(ort1);
    vec_of_sublines.push_back(ort2);
    vec_of_sublines.push_back(ort3);

}

void OrtogonalLine::setLine(qreal x1, qreal y1, qreal x2, qreal y2)
{

    vec_of_sublines[0]->setLine(x1,y1, x1+(x2-x1)*0.3,y1);
    vec_of_sublines[1]->setLine(x1+(x2-x1)*0.3,y1, x1+(x2-x1)*0.3,y2);
    vec_of_sublines[2]->setLine(x1+(x2-x1)*0.3,y2, x2,y2);

}

void OrtogonalLine::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    for(auto a : vec_of_sublines){
        a->setPen(QPen((QBrush(Qt::red), 11)));
    }

}

OrtogonalLine::~OrtogonalLine()
{

}
