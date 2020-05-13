#ifndef COMPONENT_H
#define COMPONENT_H

//todo adding tri-state properties map

#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsItem>
#include <QGraphicsObject>
#include <QBrush>
#include <QObject>
#include <QAction>
#include <QMenu>
#include <QInputDialog>
#include <QGridLayout>
#include <QTableWidget>
#include <QComboBox>
#include <QDebug>


class Component;
//class ILine;

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

    void add_line(QGraphicsItem*);
    void remove_line(QGraphicsItem*);
protected:
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override;
    QVector<QGraphicsItem*> vec_of_connections = {};
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
        void setPixmap(const QString &ico);
        void setParent(Component* parent);
        QString iconPath();
    private:
        QString _icon_path = "";
    };

public:
    explicit Component(QGraphicsObject * parent = nullptr);
    ~Component() override;

    void add_pin(const qreal &x, const qreal &y);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    void set_pixmap(const QString&);
    QString icon_path() const;
    QString type = "";
    QVector<Pin *> vec_of_pins = {};
    QMap<QString, QString> tri_states_map = {};
    void add_tri_state_prop(const QString& name, const QString& states);
    void remove_tri_state_prop(const QString& name);
    void make_pins_movable();
    void block_pin_moving();
    //test
    void signal_test(int a);

public slots:
    void rotate(int angle);

signals:
    void pin_hover_signal(Pin*);

protected:
    M_pixmap * pic = new M_pixmap(this);

private:
    int value = 0;
    QGraphicsSimpleTextItem m_text;
};
#endif // COMPONENT_H
