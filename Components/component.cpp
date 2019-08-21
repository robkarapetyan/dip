#include "component.h"
#include <QBrush>
#include <QAction>
#include <QMenu>

Component::Component(QGraphicsObject *parent) : QGraphicsObject(parent)
{
    this->setFlag(ItemIsMovable);
   // this->setFiltersChildEvents(false);


   // QObject::connect(this->pic,SIGNAL(triggered)
}

Component::~Component()
{

}

void Component::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction("COmponent");
    menu.addAction("-----------");
    menu.addAction("-ssssdd------");

    menu.exec();
    //Component::contextMenuEvent(event);
}


void Component::rotate(int angle)
{
    this->pic.setRotation(angle);
}

void Component::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{}


QRectF Component::boundingRect() const
{
    return QRectF();
}
