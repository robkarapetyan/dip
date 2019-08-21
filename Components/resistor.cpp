#include "resistor.h"
#include <QBrush>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>


Resistor::Resistor(QGraphicsObject *parent)
{

    Pin * pin1 = new Pin;
    Pin * pin2 = new Pin;
    this->setAcceptHoverEvents(true);
    this->setFlag(ItemIsMovable);


    this->setFiltersChildEvents(false);
    this->pic.setPixmap(QPixmap("C:/Users/Rob/Documents/diplom_beginning/icons/resh.ico"));

    this->pic.setFlag(ItemIsMovable);
    this->pic.setAcceptHoverEvents(true);
    this->pic.setParentItem(this);

    pin1->setParentItem(&this->pic);
    pin2->setParentItem(&this->pic);
    pin1->setRect(23,10,5,5);
    pin2->setRect(-3,10,5,5);
    pin1->setBrush(QBrush(Qt::black));
    pin2->setBrush(QBrush(Qt::black));

    this->vec_of_pins.push_back(pin1);
    this->vec_of_pins.push_back(pin2);
}

Resistor::~Resistor()
{}

void Resistor::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(". . .");
    menu.addAction("properties");

    menu.exec();
    Component::contextMenuEvent(event);
}

void Resistor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{}


QRectF Resistor::boundingRect() const
{
    return QRectF();
}

