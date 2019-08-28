#include "m_pixmap.h"
#include <QMenu>
#include <QObject>


M_pixmap::M_pixmap(QGraphicsPixmapItem *)
{

}

M_pixmap::~M_pixmap()
{

}

void M_pixmap::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
    QMenu* menu = new QMenu;
        QAction *ro90 = menu->addAction("Rotate 90");
        menu->exec(event->screenPos());

}/**/

