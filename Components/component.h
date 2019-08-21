#ifndef COMPONENT_H
#define COMPONENT_H

#include <QObject>
#include "tools/pin.h"
#include "tools/m_pixmap.h"

class Component: public QGraphicsObject
{

public:
    Component(QGraphicsObject * parent = nullptr);
   // Component(QWidget * widg = nullptr);
    ~Component() override;

    //void add_pin(const QPoint &point);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent * event) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
public slots:
    void rotate(int angle);

protected:
    QVector<Pin *> vec_of_pins = {};
    M_pixmap pic;
};
#endif // COMPONENT_H
