#include "capacitor.h"
#include <QBrush>

Capacitor::Capacitor(QGraphicsObject *parent)
{
    Pin * pin1 = new Pin;
    Pin * pin2 = new Pin;
    this->setAcceptHoverEvents(true);
    this->setFlag(ItemIsMovable);


    this->setFiltersChildEvents(false);
    this->pic->setPixmap(QPixmap("/home/rob/Desktop/cc/caph.png"));

    this->pic->setFlag(ItemIsMovable);
    this->pic->setAcceptHoverEvents(true);
    this->pic->setParentItem(this);

    pin1->setParentItem(this->pic);
    pin2->setParentItem(this->pic);
    pin1->setRect(23,10,6,6);
    pin2->setRect(-3,10,6,6);
    pin1->setBrush(QBrush(Qt::black));
    pin2->setBrush(QBrush(Qt::black));

    this->vec_of_pins.push_back(pin1);
    this->vec_of_pins.push_back(pin2);
}

Capacitor::~Capacitor()
{

}

void Capacitor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}

QRectF Capacitor::boundingRect() const
{

}


