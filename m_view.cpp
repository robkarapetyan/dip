#include "m_view.h"
#include "Components/resistor.h"
#include <QDebug>


M_view::M_view(QWidget *parent):QGraphicsView(parent)
{
    //this->setContextMenuPolicy(Qt::ActionsContextMenu	);

}

M_view::~M_view()
{

}


void M_view::received_options(QGraphicsItem *, QAction *)
{

}

/* void M_view::contextMenuEvent(QContextMenuEvent *event)
{

    QGraphicsView::contextMenuEvent(event);
}
*/


void M_view::mousePressEvent(QMouseEvent *event)
{

    if(event->button() == Qt::LeftButton)
    {
        if(itemAt(event->pos()))
        {
            //this->setCursor(Qt::ClosedHandCursor);
        }
        else
        {
            switch (this->comp)
            {
                case Components::resistor:
                {
                    Resistor* restmp = new Resistor;
                    restmp->setPos(event->pos().x()-40,event->pos().y()-20);
                    this->scene()->addItem(restmp);
                    break;
                }
                case Components::capacitor:
                {
                    Capacitor* captmp = new Capacitor;
                    captmp->setPos(event->pos().x()-40,event->pos().y()-20);
                    this->scene()->addItem(captmp);
                    break;
                }
                case Components::diode:
                    break;
                case Components::inductor:
                    break;
            default: break;
            }
        }
    }
    QGraphicsView::mousePressEvent(event);
}

void M_view:: resistor_received()
{
    this->comp = Components::resistor;
}

void M_view::capacitor_received()
{
    this->comp = Components::capacitor;
}
