#ifndef FLATLINE_H
#define FLATLINE_H

#include "iline.h"
#include "ConnectionController/lineitems/flatlineitem.h"
#include <QGraphicsScene>

//class-container for flat line

class FlatLine : public ILine
{
public:
    FlatLine(Pin* a, Pin* b);
    void setLine(qreal x1, qreal y1, qreal x2, qreal y2) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event) override;


    ~FlatLine() override;
private:
    FlatLineItem _line = {};
};

#endif // FLATLINE_H
