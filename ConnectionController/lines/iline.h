#ifndef ILINE_H
#define ILINE_H
#include <QGraphicsItem>

#include "Components/tools/component.h"

class ILine : public QGraphicsItem
{
public:
    ILine(Pin* a, Pin* b);
    ~ILine() override;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    virtual void setLine(qreal x1, qreal y1, qreal x2, qreal y2) = 0;
    Pin* start = nullptr;
    Pin* end = nullptr;


};

#endif // ILINE_H
