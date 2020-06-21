#include "port.h"
#include <QPainter>
#include <QDebug>
#include "ConnectionController/lines/iline.h"

Port::Port()
{
    this->setFlag(ItemIsMovable);
//    this->setFiltersChildEvents(true);
//    this->m_pin.setFlag(ItemIsMovable);
    m_pin->setParentItem(this);
//    m_pin->installSceneEventFilter(this);
    m_pin->setRect(0,0,6,6);
    m_pin->moveBy(1,1);
    this->m_pin->setFlag(ItemIgnoresParentOpacity);

    _signature = new QGraphicsSimpleTextItem(this);
    _signature->setText( m_pin->getSignature());
    _signature->setPos(-10,-15);
    _signature->setEnabled(false);
//    this->scenep
}

Port::~Port(){}

void Port::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{
//    painter->setBrush(QBrush(Qt::green));
//    painter->setPen(Qt::black);
//    painter->drawRect(this->boundingRect().x(),this->boundingRect().y(),this->boundingRect().width() + 10, this->boundingRect().height() + 10);
}

QRectF Port::boundingRect() const
{
    return this->childrenBoundingRect();
}

void Port::setrect(qreal ax, qreal ay, qreal w, qreal h)
{
    m_pin->setRect(ax,ay,w,h);
}

QPointF Port::scenePos() const
{
    return m_pin->scenePos();
}

void Port::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mousePressEvent(event);
}

void Port::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu* menu = new QMenu;
    QAction * act1 = menu->addAction("change signature");
    QAction * act2 = menu->addAction("delete");



    connect(act1, &QAction::triggered, [this](){
        bool ok;

        QString i = QInputDialog::getText(nullptr, tr("Set signature"),
                                                      tr("signature"), QLineEdit::EchoMode::Normal, QString("smtg"), &ok);
        if (ok)
        {
            if(i!= ""){
                m_pin->setSignature(i);
                _signature->setText(i);
                if(!m_pin->vec_of_connections.isEmpty()){
                    for (auto j : m_pin->vec_of_connections){
                        auto a = dynamic_cast<ILine*>(j);
                        if(a){
                            if(a->start == m_pin){
                                a->end->setSignature(i);
                            }else if(a->end == m_pin){
                                a->start->setSignature(i);
                            }
                        }
                    }
                }
            }
        }
    });
    connect(act2, &QAction::triggered, [this](){

        if(!m_pin->vec_of_connections.isEmpty()){
            for (auto i : m_pin->vec_of_connections){
                if(i)
                    delete i;
            }
        }

        delete this;
    });
    menu->exec(event->screenPos());

}
