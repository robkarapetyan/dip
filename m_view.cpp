#include "m_view.h"
#include "Components/resistor.h"
#include <QDebug>
#include <QPair>
#include <QGuiApplication>
#include "Components/tools/component.h"
#include "Components/tools/port.h"
#include "mainwindow.h"
#include "Tools/ActionController/actions/act.h"
#include "Tools/ActionController/actions/actionadd.h"
//#include "Tools/ActionController/actions/actionremove.h"


M_view::M_view(QWidget *parent):QGraphicsView(parent)
{
    conncontroller = new Connection_controller(this);
    connect(conncontroller, SIGNAL(item_created(QGraphicsItem*)), this, SLOT(add_received_lineItem(QGraphicsItem*)));
    actcontroller = new ActionController;
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
    if(dragMode() == QGraphicsView::ScrollHandDrag){
        QGraphicsView::mousePressEvent(event);
        return;
    }
//    auto mainwindowptr = dynamic_cast<MainWindow*>(this->parent()->parent());
    if(QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ShiftModifier) && !itemAt(event->pos()))
    {
        setDragMode(QGraphicsView::ScrollHandDrag);
    }
    else if(event->button() == Qt::LeftButton)
    {
         switch (this->mode)
         {
             case ActiveMode::component:
             {
                 if(!itemAt(event->pos())){
                     QPointF pt = mapToScene(event->pos());
                     Component* cmp = new Component(*active_component);
                     cmp->setPos(pt.x() - 12,pt.y() - 12);

                     M_Action* add_act = new ActionAdd(cmp, this);
                     actcontroller->Do(add_act);

                     QString key = cmp->get_spice_form().split(" ").front().remove("'",Qt::CaseSensitive);

                     QVector<QString> vector = names_map[key];

                     if(vector.isEmpty()){
                         vector.push_back(QString(vector.size()));
                         names_map.insert(key,vector);
                     }else{
                         names_map[key].push_back(QString(names_map[key].size()));
                     }
                     cmp->setProperty("name", QString::number(names_map[key].size()));
//                     this->scene()->addItem(cmp);
                     connect(cmp, SIGNAL(remove_signal(Component*)), this, SLOT(remove_component_slot(Component*)));
                     connect(cmp, SIGNAL(pin_hover_signal(Pin*)), conncontroller, SLOT(receiving_pin(Pin *) ));
                 }
                 break;
              }
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

//    qDebug() << this->scene()->items().size();
//    for (auto i : this->scene()->items()){
//        if(auto jj = dynamic_cast<Component*>(i)){
//            qDebug() << "yes it is component" << jj->flags();
//        }
//    }
    QGraphicsView::mousePressEvent(event);
}

void M_view::mouseReleaseEvent(QMouseEvent *event)
{
    if(!QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ShiftModifier) && !grabFlag)
    {
        setDragMode(QGraphicsView::NoDrag);
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void M_view::resizeEvent(QResizeEvent *event)
{

    QGraphicsView::resizeEvent(event);
}

QStringList M_view::get_spice()
{
    QStringList stlist = {};
    for ( auto i : scene()->items()){
        Component* cmp = dynamic_cast<Component*>(i);
        if(cmp){
            if(cmp->property("spice_form").isValid() && cmp->property("spice_form").toString() != "")
            {
                stlist.push_back(cmp->get_spice_form());
            }
        }
    }
    return stlist;
}

void M_view::set_to_(const ActiveMode &a)
{
    mode = a;
}

//void M_view::setGrid_enabled(bool a)
//{
//    if(a){
//        if(!gridgroup){
//            gridgroup = new QGraphicsItemGroup;
//            for (int x=0; x<=500; x+=50)
//                gridgroup->addToGroup(new QGraphicsLineItem(x,0,x,500, gridgroup));
//    //        scene()
//            // Now add the horizontal lines, paint them green
//            for (int y=0; y<=500; y+=50)
//                gridgroup->addToGroup(new QGraphicsLineItem(0,y,500,y,  gridgroup));
//        }
//        gridgroup->setActive(false);
//        for (auto i : gridgroup->childItems()){
//            i->setActive(false);
//        }
//        gridgroup->setVisible(true);
//        scene()->addItem(gridgroup);
//        return;
//    }
//    else
//        gridgroup->setVisible(false);
//}



//-------------------------------- slots ----------------------

void M_view::add_received_lineItem(QGraphicsItem *a)
{
    this->scene()->addItem(a);
}

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

void M_view::component_received(Component *cmp)
{
    if(cmp){
        set_to_(ActiveMode::component);
        active_component = cmp;
        if(cmp->objectName() == "ground"){
            active_component->vec_of_pins[0]->setSignature("0");
        }
    }
    /*if (!action->isSeparator() && !action->isWidgetType())
    {
        if(action->text() == "Resistor")
        {
            this->mode = ActiveMode::resistor;
        }
        else if(action->text() == "Capacitor")
        {
            this->mode = ActiveMode::capacitor;
            // switching state of (graphicsView->comp) to (Components::capacitor)
        }
        else if(action->text() == "Inductor")
        {
            // switching state of (graphicsView->comp) to (Components::inductor);
        }
        else if(action->text() == "Diode")
        {
            // switching state of (graphicsView->comp) to (Components::diode);
        }
        else {
            // switching state of (graphicsView->comp) to (Components::none);
        }
    }*/
}

void M_view::remove_component_slot(Component *cmp)
{
    M_Action* remove_act = new ActionRemove(cmp, this);
    actcontroller->Do(remove_act);
}
