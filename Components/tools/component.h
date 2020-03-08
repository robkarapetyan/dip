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
    //Component* parentptr = nullptr;
public:
    //Pin() = default;
    Pin(Component* ptr = nullptr);
    ~Pin();
    void contextMenuEvent(QGraphicsSceneContextMenuEvent * event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event) override;
protected:
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override;
};

class Component: public QGraphicsObject
{
    Q_OBJECT
    //------------------Pixmap---------------
    class M_pixmap : public QGraphicsPixmapItem
    {
        Component* parentptr = nullptr;
    public:
         explicit M_pixmap(Component * ptr = nullptr);
        ~M_pixmap() override;
        void contextMenuEvent(QGraphicsSceneContextMenuEvent *)  override;
//        void moveevent
        void setParent(Component* parent);
    };

    int value = 0;
    QGraphicsSimpleTextItem m_text;
public:
    explicit Component(QGraphicsObject * parent = nullptr);
    ~Component() override;


    void add_pin(const QRectF &rect);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
//    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    QRectF boundingRect() const override;
    QString type = "";
    QPointF scenePos() const;

    //test
    void signal_test(int a);

public slots:
    void rotate(int angle);
signals:
    void pin_hover_signal(Pin*);

protected:

    QVector<Pin *> vec_of_pins = {};
    M_pixmap * pic = new M_pixmap(this);
};
#endif // COMPONENT_H
