#include "m_view.h"
#include "Components/resistor.h"
#include <QDebug>
#include <QGuiApplication>
#include "Components/tools/component.h"
#include "Components/tools/port.h"
#include "mainwindow.h"

M_view::M_view(QWidget *parent):QGraphicsView(parent)
{
    conncontroller = new Connection_controller(this);
    connect(conncontroller, SIGNAL(item_created(QGraphicsItem*)), this, SLOT(add_received_lineItem(QGraphicsItem*)));
}

M_view::~M_view()
{

}

//------------------------------- mouse events ---------------


void M_view::wheelEvent(QWheelEvent *event)
{
    if(QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ControlModifier))
    {
        this->centerOn(mapToScene(event->pos()));
//        qDebug() << m_scaling;
        if(event->delta() >= 0){
           if(m_scaling <= 140){
               emit scaling_sig(m_scaling + 10);
           }
        }
        else{
           if(m_scaling >= 60){
               emit scaling_sig(m_scaling - 10);
           }
        }

    }
    else{
        QGraphicsView::wheelEvent(event);
        return;
    }
}

void M_view::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
}

void M_view::mousePressEvent(QMouseEvent *event)
{
//    auto mainwindowptr = dynamic_cast<MainWindow*>(this->parent()->parent());
    if(QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ShiftModifier) && !itemAt(event->pos()))
    {
        setDragMode(QGraphicsView::ScrollHandDrag);
    }
    else if(event->button() == Qt::LeftButton)
    {
         switch (this->mode)
         {
             case ActiveMode::resistor:
             {
                 if(!itemAt(event->pos())){
                     QPointF pt = mapToScene(event->pos());
                     Resistor* restmp = new Resistor;
                     restmp->setPos(pt.x() - 12,pt.y() - 12);
                     this->scene()->addItem(restmp);
                     connect(restmp, SIGNAL(pin_hover_signal(Pin*)), conncontroller, SLOT(receiving_pin(Pin *) ));
                 }
                 break;
              }
              case ActiveMode::capacitor:
              {
                  if(!itemAt(event->pos())){
                      QPointF pt = mapToScene(event->pos());
                      Capacitor* captmp = new Capacitor;
                      captmp->setPos(pt.x() - 12,pt.y() - 12);
                      this->scene()->addItem(captmp);
                      connect(captmp, SIGNAL(pin_hover_signal(Pin*)), conncontroller, SLOT(receiving_pin(Pin *) ));

                  }
                  break;
              }
              case ActiveMode::diode:
                  break;
              case ActiveMode::inductor:
                  break;
              case ActiveMode::port:
              {
                if(!itemAt(event->pos())){
                    QPointF pt = mapToScene(event->pos());
                    Port* pintmp = new Port;
                    pintmp->setPos(pt.x() +1,pt.y() +1);
                    this->scene()->addItem(pintmp);
                    connect(pintmp, SIGNAL(pin_hover_signal(Pin*)), conncontroller, SLOT(receiving_pin(Pin *) ));

                }
                break;
              }

              default: break;
         }
    }

    qDebug() << this->scene()->items().size();
//    for (auto i : this->scene()->items()){
//        if(dynamic_cast<Component*>(i)){
//            qDebug() << "yes it is component";
//        }
//    }
    QGraphicsView::mousePressEvent(event);
}

void M_view::mouseReleaseEvent(QMouseEvent *event)
{
    setDragMode(QGraphicsView::NoDrag);
    QGraphicsView::mouseReleaseEvent(event);
}

void M_view::resizeEvent(QResizeEvent *event)
{

    QGraphicsView::resizeEvent(event);
}

void M_view::set_to_(const ActiveMode &a)
{
    mode = a;
}

void M_view::add_received_lineItem(QGraphicsItem *a)
{
    this->scene()->addItem(a);
}

//-------------------------------- slots ----------------------

void M_view::change_scale(int new_val)
{
    if(new_val > m_scaling){
        m_scaling += 10;
        this->scale(1.1,1.1);
    }
    else{
        m_scaling -= 10;
        this->scale(0.91, 0.91);
    }
}

void M_view:: resistor_received()
{
    this->mode = ActiveMode::resistor;
}

void M_view::capacitor_received()
{
    this->mode = ActiveMode::capacitor;
}
