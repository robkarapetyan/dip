#ifndef COMPONENT_H
#define COMPONENT_H

#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsItem>
#include <QGraphicsObject>
#include <QBrush>
#include <QObject>
#include <QAction>
#include <QMenu>
#include <QInputDialog>
#include <QDebug>

class Component;

class Pin : public QGraphicsRectItem
{
    friend class Component;
    //Component* parentptr = nullptr;
public:
    //Pin() = default;
    Pin();
    ~Pin();
    void contextMenuEvent(QGraphicsSceneContextMenuEvent * event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event) override;
protected:
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override;

};


//------------------Component---------------

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

public:
    explicit Component(QGraphicsObject * parent = nullptr);
    ~Component() override;

    void add_pin(const qreal &x, const qreal &y);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    QString type = "";

    //test
    void signal_test(int a);

public slots:
    void rotate(int angle);

signals:
    void pin_hover_signal(Pin*);

protected:
    QVector<Pin *> vec_of_pins = {};
    M_pixmap * pic = new M_pixmap(this);

private:
    int value = 0;
    QGraphicsSimpleTextItem m_text;
};
#endif // COMPONENT_H
