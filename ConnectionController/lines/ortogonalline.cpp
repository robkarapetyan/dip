#include "ortogonalline.h"

OrtogonalLine::OrtogonalLine(Pin* a, Pin* b) : ILine(a, b)
{
    setFiltersChildEvents(true);
//    this->setFlag(ItemIsMovable);

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

    ort1->set_siblings(nullptr, ort2);
    ort2->set_siblings(ort1, ort3);
    ort3->set_siblings(ort2, nullptr);

}

void OrtogonalLine::setLine(qreal x1, qreal y1, qreal x2, qreal y2)
{

//        vec_of_sublines[1]->setPos(QPointF(x1+(x2-x1)*0.3,y1));
//        vec_of_sublines[2]->setPos(QPointF(x1+(x2-x1)*0.3,y2));

    ////////////////

    vec_of_sublines[1]->setPos((x2-x1)*0.3,0);

    vec_of_sublines[2]->setOrientation(Qt::Orientation::Vertical);
    vec_of_sublines[2]->setPos((x2-x1)*0.3, 0);
    vec_of_sublines[2]->setOrientation(Qt::Orientation::Horizontal);
    vec_of_sublines[2]->setPos(0, y2 - y1);

    ////////////////

    vec_of_sublines[0]->setLine(0,0, (x2-x1)*0.3,0);
    vec_of_sublines[1]->setLine(0,0, 0,y2 - y1);
    vec_of_sublines[2]->setLine(0,0, (x2-x1)*0.7,0);


//    vec_of_sublines[0]->setLine(x1,y1, x1+(x2-x1)*0.3,y1);
//    vec_of_sublines[1]->setLine(x1+(x2-x1)*0.3,y1, x1+(x2-x1)*0.3,y2);
//    vec_of_sublines[2]->setLine(x1+(x2-x1)*0.3,y2, x2,y2);

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
