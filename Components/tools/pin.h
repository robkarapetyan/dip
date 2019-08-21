#ifndef PIN_H
#define PIN_H
#include <QGraphicsRectItem>


class Pin : public QGraphicsRectItem
{
public:
    Pin();
    ~Pin();
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

};

#endif // PIN_H
