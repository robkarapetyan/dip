#ifndef CAPACITOR_H
#define CAPACITOR_H
#include "Components/tools/component.h"

class Capacitor : public Component
{
private:
    int capacitance = 0;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
public:
    explicit Capacitor(QGraphicsObject *parent = nullptr);
   ~Capacitor() override;

    int get_capacitance();
public slots:

};

#endif // CAPACITOR_H
