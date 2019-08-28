#ifndef M_PIXMAP_H
#define M_PIXMAP_H

#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsPixmapItem>

class M_pixmap : public QGraphicsPixmapItem
{
public:

     M_pixmap(QGraphicsPixmapItem * parent = nullptr);
    ~M_pixmap() override;


    void contextMenuEvent(QGraphicsSceneContextMenuEvent *) override ;

};
#endif // M_PIXMAP_H
