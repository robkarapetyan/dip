#ifndef CAPACITOR_H
#define CAPACITOR_H
#include "Components/component.h"

class Capacitor : public Component
{
private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
public:
    explicit Capacitor(QGraphicsObject *parent = nullptr);
   ~Capacitor() override;
public slots:

};

#endif // CAPACITOR_H
