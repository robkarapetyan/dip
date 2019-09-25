#include "component.h"
#include <QBrush>
#include <QAction>
#include <QMenu>
#include <QInputDialog>
#include <QDebug>

Component::Component(QGraphicsObject *parent) : QGraphicsObject(parent)
{
    this->setFlag(ItemIsMovable);
    this->setFiltersChildEvents(false);
    this->pic->setFlag(ItemIsMovable);
    this->pic->setParentItem(this);
    this->pic->setFiltersChildEvents(true);

}

Component::~Component()
{

}

void Component::add_pin(const QRectF &rect)
{
    Pin * pin1 = new Pin(this);
    pin1->setRect(rect);
    pin1->setBrush(QBrush(Qt::black));
    this->vec_of_pins.push_back(pin1);
    pin1->setParentItem(this->pic);
}

/**/

void Component::rotate(int angle)
{
    QTransform txf = QTransform();
    txf.translate((12+12),(0));
    txf.rotate(angle, Qt::ZAxis);
    txf.translate(0,0);
    this->setTransform(txf, true);
}

void Component::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{}


QRectF Component::boundingRect() const
{
    return QRectF();
}

//--------------------------- PIN ----------------------------------

Component::Pin::Pin(Component* parent){

    this->setParent(parent);
    this->setAcceptHoverEvents(true);
}

Component::Pin::~Pin()
{

}

void Component::Pin::contextMenuEvent(QGraphicsSceneContextMenuEvent *)
{
    //QGraphicsRectItem::contextMenuEvent(event);
}

void Component::Pin::mousePressEvent(QGraphicsSceneMouseEvent *event)//pin clicked
{
    if(event->button() == Qt::LeftButton)
    {
        emit this->parentptr->pin_hover_signal(event->pos());
        qDebug() << QString("%1").arg(event->scenePos().x());
    }

    QGraphicsRectItem::mousePressEvent(event);
}

void Component::Pin::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    this->setBrush(QBrush(Qt::red));
    QGraphicsRectItem::hoverEnterEvent(event);
}

void Component::Pin::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    this->setBrush(QBrush(Qt::black));
    QGraphicsRectItem::hoverLeaveEvent(event);
}

void Component::Pin::setParent(Component *parent)
{
    this->parentptr = parent;
}

//---------------------------- Pixmap------------------------------

Component::M_pixmap::M_pixmap(Component *ptr)
{
    this->setParent(ptr);
}

Component::M_pixmap::~M_pixmap()
{

}

void Component::M_pixmap::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
    QMenu * menu = new QMenu;
    QAction * act1 = menu->addAction("rotate");
    QAction * act2 =  menu->addAction("remove");
    QAction * act3 =  menu->addAction("properties");

    QWidget * widget = new QWidget;
    widget->move(event->screenPos().rx(),event->screenPos().ry());

    connect(act1, &QAction::triggered, [this](){
        QTransform txf = QTransform();
        qreal a = this->rotation() + 90;
        txf.translate((12+12),(0));
        txf.rotate(a, Qt::ZAxis);
        txf.translate(0,0);
        this->setTransform(txf, true);
    });
    connect(act2, &QAction::triggered, [this](){
        this->parentptr->~Component();
    });
    connect(act3, &QAction::triggered, [this,widget](){

              if(this->parentptr->objectName() == "resistor")
              {   qDebug("resisting");
                  bool ok;
                    int i = QInputDialog::getInt(widget, tr("Set Resistance"),
                                                                 tr("resistance ( OHM )"), this->parentptr->value,
                                                                 0,2000000,1, &ok);
                    if (ok)
                    {
                        this->parentptr->value = i;
                    }
              }
              else if(this->parentptr->objectName() == "capacitor"){
                  qDebug("capaciting");
                  bool ok;
                    int i = QInputDialog::getInt(widget, tr("Set Capacitance"),
                                                                  tr("capacitance ( pF )"), this->parentptr->value,
                                                                  0,2000000,1, &ok);
                     if (ok)
                     {
                       this->parentptr->value = i;
                     }
              }
             // i - result taken from inputdialog
    });


    menu->exec(QPoint(event->screenPos().rx()+3,event->screenPos().ry()));
    QGraphicsPixmapItem::contextMenuEvent(event);
}

void Component::M_pixmap::setParent(Component *parent)
{
    this->parentptr = parent;
}
