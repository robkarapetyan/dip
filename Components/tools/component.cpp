#include <QBrush>
#include <QAction>
#include <QMenu>
#include <QInputDialog>
#include <QDebug>
#include <QGraphicsScene>
#include "port.h"
#include "component.h"
#include "ConnectionController/lines/flatline.h"
#include "ConnectionController/lines/ortogonalline.h"
#include "ConnectionController/lines/iline.h"

Component::Component(QGraphicsObject *parent) : QGraphicsObject(parent)
{
    this->setFlag(ItemIsMovable);
    this->pic->setFlag(ItemIgnoresParentOpacity);
    this->pic->setParentItem(this);

//    connect(this, SIGNAL(pin_hover_signal(int)), this, SLOT(test_of_pin_click(int)));
//    m_text.setParentItem(this->pic);
//    m_text.setPos(4,-8);
//    //m_text.setFlag(GraphicsItemFlag::ItemIsMovable);
//    m_text.setPen(QPen(QColor(Qt::red)));
//    m_text.setText("0");
//    m_text.setFont(QFont("arial",8,-1,false));
}

Component::~Component()
{
}

void Component::add_pin(const qreal &x, const qreal &y)
{
    Pin * pin1 = new Pin;
    pin1->moveBy(x,y);
    //this->vec_of_pins.push_back(pin1);
    pin1->setParentItem(this->pic);
}


void Component::rotate(int angle)
{
    QTransform txf = QTransform();
    txf.translate((12+12),(0));
    txf.rotate(angle, Qt::ZAxis);
    txf.translate(0,0);
    this->setTransform(txf, true);
}



void Component::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{
}

//void Component::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    for( Pin* pn : vec_of_pins){
//        qDebug() << pn->scenePos();
//    }

//    qDebug() << this << "scp" << event->scenePos();
//}


QRectF Component::boundingRect() const
{
    return this->childrenBoundingRect();
}


//--------------------------- PIN ----------------------------------

Pin::Pin(){
    setRect(0,0,6,6);
//    setFlag(ItemIgnoresParentOpacity);
    setBrush(QBrush(Qt::black));
    this->setAcceptHoverEvents(true);
    setFlag(ItemSendsScenePositionChanges);
}

Pin::~Pin()
{
}

void Pin::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QGraphicsRectItem::contextMenuEvent(event);
}

void Pin::mousePressEvent(QGraphicsSceneMouseEvent *event)//pin clicked
{
    if(event->button() == Qt::LeftButton)
    {
        if(dynamic_cast<Component*>(this->parentItem()->parentItem())){
            Component* a = dynamic_cast<Component*>(this->parentItem()->parentItem());
            emit a->pin_hover_signal(this);
        }else if(dynamic_cast<Port*>(this->parentItem())){
            Port* a = dynamic_cast<Port*>(this->parentItem());
            emit a->pin_hover_signal(this);
        }
    }
    QGraphicsRectItem::mousePressEvent(event);
}

void Pin::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    this->setBrush(QBrush(Qt::red));
    QGraphicsRectItem::hoverEnterEvent(event);
}

void Pin::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    this->setBrush(QBrush(Qt::black));
    QGraphicsRectItem::hoverLeaveEvent(event);
}

void Pin::add_line(QGraphicsItem * line)
{
    this->vec_of_connections.push_back(line);
}

void Pin::remove_line(QGraphicsItem * line)
{
    vec_of_connections.removeAll(line);
}

QVariant Pin::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(this->parentItem())
    this->parentItem()->update(this->parentItem()->boundingRect());

    if(change == QGraphicsItem::ItemScenePositionHasChanged){

        for (auto i : vec_of_connections){
            if(auto j = dynamic_cast<FlatLine*>(i)){
                if(j->start == this){
                    j->setLine(value.toPointF().x(), value.toPointF().y(), j->end->scenePos().x(), j->end->scenePos().y());
                }
                if(j->end == this){
                    j->setLine(j->start->scenePos().x(), j->start->scenePos().y(), value.toPointF().x(), value.toPoint().y());

                }
            }
            if(auto j = dynamic_cast<OrtogonalLine*>(i)){
                if(j->start == this){
                    j->setLine(value.toPointF().x(), value.toPointF().y(), j->end->scenePos().x(), j->end->scenePos().y());
                }
                if(j->end == this){
                    j->setLine(j->start->scenePos().x(), j->start->scenePos().y(), value.toPointF().x(), value.toPoint().y());

                }
            }

        }
    }

    return QGraphicsRectItem::itemChange(change,value);
}



//---------------------------- Pixmap------------------------------

Component::M_pixmap::M_pixmap(Component *ptr)
{
    this->parentptr = ptr;
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
        //this->scene()->removeItem(this->parentItem());
        this->parentptr->~Component();
    });
    connect(act3, &QAction::triggered, [this,widget](){

         if(this->parentObject()->objectName() == "resistor")
         {
              bool ok;
              int i = QInputDialog::getInt(widget, tr("Set Resistance"),
                                                            tr("resistance ( OHM )"), this->parentptr->value,
                                                             0,2000000,1, &ok);
              if (ok)
              {
                 if(i != 0){
                    this->parentptr->value = i;
                 }

              }
         }
         else if(this->parentObject()->objectName() == "capacitor"){
              qDebug("capaciting");
              bool ok;
              int i = QInputDialog::getInt(widget, tr("Set Capacitance"),
                                                              tr("capacitance ( pF )"), this->parentptr->value,
                                                              0,2000000,1, &ok);
              if (ok)
              {
                 if(i != 0){
                    this->parentptr->value = i;
                 }
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
