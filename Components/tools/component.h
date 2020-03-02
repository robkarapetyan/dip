#ifndef COMPONENT_H
#define COMPONENT_H

#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsPixmapItem>
#include <QBrush>
#include <QObject>

//#include "Components/tools/pin.h"
//#include "Components/tools/m_pixmap.h"
class Component;

class Pin : public QGraphicsRectItem
{
    friend class Component;
    Component* parentptr = nullptr;
public:
    explicit Pin(Component* ptr = nullptr);
    ~Pin();
    void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
    void setParent(Component* parent);
};

class Component: public QGraphicsObject
{
    //------------------Pixmap---------------
    class M_pixmap : public QGraphicsPixmapItem
    {
        Component* parentptr = nullptr;
    public:
         explicit M_pixmap(Component * ptr = nullptr);
        ~M_pixmap() override;
        void contextMenuEvent(QGraphicsSceneContextMenuEvent *)  override;
        void setParent(Component* parent);
    };

    int value = 0;
public:
    explicit Component(QGraphicsObject * parent = nullptr);
    ~Component() override;


    void add_pin(const QRectF &rect);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    QString type = "";
    QPointF scenePos() const;
public slots:
    void rotate(int angle);
signals:
    void pin_hover_signal(const QPointF &){}

protected:
    QVector<Pin *> vec_of_pins = {};
    M_pixmap * pic = new M_pixmap(this);
};
#endif // COMPONENT_H
