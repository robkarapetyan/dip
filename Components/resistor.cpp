#include "resistor.h"
#include <QBrush>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>


Resistor::Resistor(QGraphicsObject * parent): Component(parent)
{
    this->setObjectName("resistor");

    this->setAcceptHoverEvents(true);
    this->setFlag(ItemIsMovable);

    this->setFiltersChildEvents(false);
    this->pic->setPixmap(QPixmap("/home/rob/Public/icons/resh.ico"));

    //this->pic->setFlag(ItemIsMovable);
    //this->pic->setAcceptHoverEvents(true);
    //this->pic->setParentItem(this);

    this->add_pin(QRectF(23,10,6,6));
    this->add_pin(QRectF(-3,10,6,6));

    //this->vec_of_pins.push_back(pin1);
    //this->vec_of_pins.push_back(pin2);
}

Resistor::~Resistor()
{}

/*void Resistor::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(". . .");
    menu.addAction("properties");

    menu.exec();
    Component::contextMenuEvent(event);
}*/

void Resistor::rotate90()
{
    this->pic->setRotation(90);
}

void Resistor::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{}


QRectF Resistor::boundingRect() const
{
    return QRectF();
}



