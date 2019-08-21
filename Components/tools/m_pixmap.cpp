#include "m_pixmap.h"
#include <QMenu>
#include <QObject>


M_pixmap::M_pixmap(QGraphicsPixmapItem * parent)
{

}

M_pixmap::~M_pixmap()
{

}
/*
void M_pixmap::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
    QMenu* menu = new QMenu;
        QAction *removeAction = menu->addAction("Remove");
        QAction *markAction = menu->addAction("Mark");
        QAction *selectedAction = menu->exec(event->screenPos());
       // QObject::connect(menu,SIGNAL(triggered(selectedAction)),this->parentWidget(),SLOT(received_options()));
               // connect(menu, SIGNAL(triggered(QAction *)),this->parentWidget(), SLOT(triggered(QAction *)));
}
*/
