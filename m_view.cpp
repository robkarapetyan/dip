#include "m_view.h"
#include "Components/resistor.h"
#include <QDebug>
#include <QGuiApplication>

M_view::M_view(QWidget *parent):QGraphicsView(parent)
{

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

        qDebug() << m_scaling;

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


    /**/
}

void M_view::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
}

void M_view::mousePressEvent(QMouseEvent *event)
{


    if(QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ShiftModifier) && !itemAt(event->pos()))
    {
        setDragMode(QGraphicsView::ScrollHandDrag);
    }
    else if(event->button() == Qt::LeftButton)
    {
         switch (this->comp)
         {
             case Components::resistor:
             {
                 if(!itemAt(event->pos())){
                     QPointF pt = mapToScene(event->pos());
                     Resistor* restmp = new Resistor;
                     restmp->setPos(pt.x() - 12,pt.y() - 12);
                     this->scene()->addItem(restmp);
                 }
                 break;
              }
              case Components::capacitor:
              {
                  if(!itemAt(event->pos())){
                      QPointF pt = mapToScene(event->pos());
                      Capacitor* captmp = new Capacitor;
                      captmp->setPos(pt.x() - 12,pt.y() - 12);
                      this->scene()->addItem(captmp);
                  }
                  break;
              }
              case Components::diode:
                  break;
              case Components::inductor:
                  break;
              default: break;
         }
    }
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
    this->comp = Components::resistor;
}

void M_view::capacitor_received()
{
    this->comp = Components::capacitor;
}
