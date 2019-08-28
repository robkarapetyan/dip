#ifndef RESISTOR_H
#define RESISTOR_H

#include <QGraphicsItem>
#include "Components/tools/component.h"

class Resistor: public Component
{
public:
    int resistance = 0;
    explicit Resistor(QGraphicsObject *parent = nullptr);
   ~Resistor() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
private:

    //void setRotation(QReal rot);
    //void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    //void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
public slots:
    void rotate90();

};
#endif // RESISTOR_H
