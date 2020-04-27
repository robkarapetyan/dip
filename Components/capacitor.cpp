#include "capacitor.h"
#include <QBrush>

Capacitor::Capacitor(QGraphicsObject *parent): Component(parent)
{
    this->setObjectName("capacitor");

    this->setAcceptHoverEvents(true);
//    this->setFlag(ItemIsMovable);


//    this->setFiltersChildEvents(false);
    this->pic->setPixmap("://icons/caph.png");

    //this->pic->setFlag(ItemIsMovable);
    //this->pic->setAcceptHoverEvents(true);
    //this->pic->setParentItem(this);


    this->add_pin(23,10);
    this->add_pin(-3,10);


    //this->vec_of_pins.push_back(pin1);
    //this->vec_of_pins.push_back(pin2);
}

Capacitor::~Capacitor()
{

}

int Capacitor::get_capacitance()
{
  return this->capacitance;
}

void Capacitor::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{

}

QRectF Capacitor::boundingRect() const
{
    return this->childrenBoundingRect();
}


