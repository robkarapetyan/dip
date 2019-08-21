#ifndef RESISTOR_H
#define RESISTOR_H

#include <QGraphicsItem>

#include "Components/component.h"

class Resistor: public Component
{
private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
public:
    explicit Resistor(QGraphicsObject *parent = nullptr);
   ~Resistor() override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;


};

/*class Resistor: public QGraphicsItem
{
private:
    Pin * pin1 = new Pin;
    Pin * pin2 = new Pin;
    M_pixmap * pic = new M_pixmap;
public:
    explicit Resistor(QGraphicsItem *parent = nullptr);
   ~Resistor() override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

};*/
#endif // RESISTOR_H
