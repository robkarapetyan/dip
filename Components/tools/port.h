#ifndef PORT_H
#define PORT_H
#include "component.h"


class Port : public QGraphicsObject
{
    Q_OBJECT
public:
    Port();
    ~Port() override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    void setrect(qreal ax, qreal ay,qreal w, qreal h);
    QPointF scenePos() const;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
private:
    Pin* m_pin = new Pin;
signals:
    void pin_hover_signal(Pin *);
};

#endif // PORT_H
