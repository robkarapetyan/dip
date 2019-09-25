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

void M_view::wheelEvent(QWheelEvent *event)
{
    if(QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ControlModifier))
    {

        if(event->delta() >= 0)
        {
           this->centerOn(event->posF());
           if(m_scaling <= 1.6){
                qDebug() << QString("%1").arg(m_scaling);
               //qDebug("%i", m_scaling);
               m_scaling = m_scaling + 0.1;
               this->scale(1.1,1.1);
           }
        }
        else
        {
            this->centerOn(event->posF());
            if(m_scaling >= 0.5){
                qDebug() << QString("%1").arg(m_scaling);
                //qDebug("%i", m_scaling);
                m_scaling = m_scaling - 0.1;
                this->scale(0.91, 0.91);
            }
            //this->setSceneRect(this->viewport()->x(),this->viewport()->y(),this->width(), this->scene()->height());
        }
    }
    QGraphicsView::wheelEvent(event);
}

void M_view::received_options(QGraphicsItem *, QAction *)
{

}

void M_view::mousePressEvent(QMouseEvent *event)
{

    if(event->button() == Qt::LeftButton)
    {
         switch (this->comp)
         {
             case Components::resistor:
             {
                 if(!itemAt(event->pos())){
                     Resistor* restmp = new Resistor;
                     restmp->setPos(event->globalX() - 12,event->globalY() - 12);
                     this->scene()->addItem(restmp);
                 }
                 break;
              }
              case Components::capacitor:
              {
                  if(!itemAt(event->pos())){
                      Capacitor* captmp = new Capacitor;
                      captmp->setPos(event->pos().x() - 12,event->pos().y() - 12);
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


    //if(event->button() == Qt::MiddleButton){
        if(QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ShiftModifier) && !itemAt(event->pos()))
        {
            setDragMode(QGraphicsView::ScrollHandDrag);
        }
    //}
    QGraphicsView::mousePressEvent(event);
}

void M_view::mouseReleaseEvent(QMouseEvent *event)
{
    setDragMode(QGraphicsView::NoDrag);
    QGraphicsView::mouseReleaseEvent(event);
}

void M_view::resizeEvent(QResizeEvent *event)
{
    if(scene()->width() < this->rect().width()){
        this->setSceneRect(0, 0, this->rect().width() - 5, this->rect().height() - 7);
    }else{

        this->setSceneRect(0,0,this->scene()->width(), this->scene()->height());
    }
    QGraphicsView::resizeEvent(event);
}

void M_view:: resistor_received()
{
    this->comp = Components::resistor;
}

void M_view::capacitor_received()
{
    this->comp = Components::capacitor;
}
