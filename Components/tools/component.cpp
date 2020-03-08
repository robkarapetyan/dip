#include "component.h"
#include <QBrush>
#include <QAction>
#include <QMenu>
#include <QInputDialog>
#include <QDebug>
#include "port.h"

Component::Component(QGraphicsObject *parent) : QGraphicsObject(parent)
{
//    this->installSceneEventFilter(this);
    this->setFlag(ItemIsMovable);

//    this->setHandlesChildEvents(true);

    //this->setFlag(ItemIsPanel);
    //this->setParentItem(this->pic);
    this->pic->setFlag(ItemIgnoresParentOpacity);
    //this->setFlag(ItemClipsToShape);
//    this->setFiltersChildEvents(true);
//    this->pic->setFlag(ItemIsMovable);
    this->pic->setParentItem(this);
    //this->pic->setFiltersChildEvents(true);

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

void Component::add_pin(const QRectF &rect)
{
    Pin * pin1 = new Pin;
    pin1->setRect(rect);
    //this->vec_of_pins.push_back(pin1);
    pin1->setParentItem(this);
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

QPointF Component::scenePos() const
{
    return pic->scenePos();
}

//--------------------------- PIN ----------------------------------


Pin::Pin(Component* ){
    setFlag(ItemIgnoresParentOpacity);
    setBrush(QBrush(Qt::black));
    this->setAcceptHoverEvents(true);
//    setFlag(ItemSendsScenePositionChanges);


}

Pin::~Pin()
{

}

void Pin::contextMenuEvent(QGraphicsSceneContextMenuEvent *)
{
    //QGraphicsRectItem::contextMenuEvent(event);
}

void Pin::mousePressEvent(QGraphicsSceneMouseEvent *event)//pin clicked
{
//    qDebug() << this->scenePos().x() << " " << this->scenePos().y() << event->scenePos().x();
    qDebug() << "scene pos : " << this->parentItem()->scenePos();
//    qDebug() << " pos : " << this->pos();
    qDebug() << "event scenepos : " << event->scenePos();

    if(event->button() == Qt::LeftButton)
    {
        if(dynamic_cast<Component*>(this->parentItem()->parentItem())){
            Component* a = dynamic_cast<Component*>(this->parentItem()->parentItem());
            emit a->pin_hover_signal(this);
            //qDebug() << this->scenePos().x();
            //qDebug() << "yup";
        }else if(dynamic_cast<Port*>(this->parentItem())){
            Port* a = dynamic_cast<Port*>(this->parentItem());
            emit a->pin_hover_signal(this);
            //qDebug() << "yupe";
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

QVariant Pin::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(this->parentItem())
    this->parentItem()->update(this->parentItem()->boundingRect());
    return QGraphicsRectItem::itemChange(change,value);
}



//---------------------------- Pixmap------------------------------

Component::M_pixmap::M_pixmap(Component *ptr)
{
    //this->setParent(ptr);
}

Component::M_pixmap::~M_pixmap()
{

}

void Component::M_pixmap::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
    //this->installSceneEventFilter(this);
    //this->parentItem()->setPos(this->scenePos());
    //this->parentItem()->update(this->parentItem()->childrenBoundingRect());
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
        delete this->parentptr;
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
                        if(i != 0){
                            this->parentptr->value = i;
//                            this->parentptr->m_text.setPen(QPen(QBrush(Qt::BrushStyle::NoBrush),3, Qt::PenStyle::SolidLine, Qt::PenCapStyle::SquareCap, Qt::PenJoinStyle::BevelJoin));
//                            this->parentptr->m_text.setText(QStringLiteral("%1 Ohm").arg(i));
                        }

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
                         if(i != 0){
                             this->parentptr->value = i;
//                             this->parentptr->m_text.setPen(QPen(QPen(QBrush(Qt::BrushStyle::NoBrush),3, Qt::PenStyle::SolidLine, Qt::PenCapStyle::SquareCap, Qt::PenJoinStyle::BevelJoin)));
//                             this->parentptr->m_text.setText(QStringLiteral("%1 pF").arg(i));
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
