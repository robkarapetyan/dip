#include "m_view.h"


M_view::M_view(QWidget *parent):QGraphicsView(parent)
{
    //this->setContextMenuPolicy(Qt::ActionsContextMenu	);
    this->viewport()->setFocusProxy(nullptr);
    this->setFocusProxy(nullptr);
    this->viewport()->installEventFilter(nullptr);
    this->installEventFilter(nullptr);
}

M_view::~M_view()
{

}

void M_view::received_options(QGraphicsItem *, QAction *)
{

}

void M_view::contextMenuEvent(QContextMenuEvent *event)
{
    QPointF p=event->pos();
       QGraphicsItem* item=itemAt(p.x(),p.y());
       if (item != nullptr)
       {
           qDebug("yes");
          /* QMenu menu(this);

           QMenu* submenuA = menu.addMenu( "rotate" );
           QAction * rot90 =submenuA->addAction("90");
           QAction * rot180 = submenuA->addAction("180");

           QAction * prop = menu.addAction("properties");


           connect(prop, &QAction::triggered, [](){
                qDebug("rot90");

               // emit rotate_sig(90);
           });
          // connect(prop, SIGNAL(triggered()), item , SLOT(addRemoveGraph()));

           menu.exec(event->globalPos());/*/
       }
       else {
            QGraphicsView::contextMenuEvent(event);
       }
}

void M_view::do_stuff()
{
    emit rotate_sig(90);
}/**/
