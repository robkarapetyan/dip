#include <QBrush>
#include <QAction>
#include <QMenu>
#include <QInputDialog>
#include <QDebug>
#include <QGraphicsScene>
#include "port.h"
#include "component.h"
#include "ConnectionController/lines/flatline.h"
#include "ConnectionController/lines/ortogonalline.h"
#include "ConnectionController/lines/iline.h"

Component::Component(QGraphicsObject *parent) : QGraphicsObject(parent)
{
    this->setFlag(ItemIsMovable);
    this->pic->setFlag(ItemIgnoresParentOpacity);
    this->pic->setParentItem(this);
    setProperty("rotation_angle", 0);
//    setProperty("icon", "pic->iconPath()");
//    setProperty("icoasn", "ddddd");

}

Component::Component(const Component &other)
{
    for( auto i : other.vec_of_pins){
        add_pin(i->pos().x(), i->pos().y());
    }
    tri_states_map = other.tri_states_map;

    for(auto i : other.dynamicPropertyNames()){
        setProperty(i, other.property(i));
    }
    set_pixmap(other.icon_path());
    this->setFlag(ItemIsMovable);
    this->pic->setFlag(ItemIgnoresParentOpacity);
    this->pic->setParentItem(this);
}

Component::~Component()
{
}

void Component::add_pin(const qreal &x, const qreal &y)
{
    Pin * pin1 = new Pin;
    pin1->moveBy(x,y);
    this->vec_of_pins.push_back(pin1);
    pin1->setParentItem(this->pic);

    setProperty("pins", vec_of_pins.size());
}


void Component::rotate(int angle)
{
    QTransform txf = QTransform();
    txf.translate((12+12),(0));
    txf.rotate(angle, Qt::ZAxis);
    txf.translate(0,0);
    this->setTransform(txf, true);
}


void Component::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{
}

//void Component::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    for( Pin* pn : vec_of_pins){
//        qDebug() << pn->scenePos();
//    }

//    qDebug() << this << "scp" << event->scenePos();
//}


QRectF Component::boundingRect() const
{
    return this->childrenBoundingRect();
}

void Component::set_pixmap(const QString & path)
{
    pic->setPixmap(path);
//    setProperty("icon", path);
}

QString Component::icon_path() const
{
    return pic->iconPath();
}

void Component::add_tri_state_prop(const QString &name, const QString &states)
{
    tri_states_map.insert(name + "_tri_state", states);
    tri_states_map.insert(name + "_tri_state_active", states.split(",").at(0));
}

void Component::remove_tri_state_prop(const QString &name)
{
    tri_states_map.remove(name + "_tri_state");
    tri_states_map.remove(name + "_tri_state_active");
}

void Component::make_pins_movable()
{
    for ( auto i : vec_of_pins){
        i->setFlag(ItemIsMovable, true);
    }
}

void Component::block_pin_moving()
{
    for ( auto i : vec_of_pins){
        i->setFlag(ItemIsMovable, false);
    }
}


//--------------------------- PIN ----------------------------------

Pin::Pin(){
    setRect(0,0,6,6);
//    setFlag(ItemIgnoresParentOpacity);
    setBrush(QBrush(Qt::black));
    this->setAcceptHoverEvents(true);
    setFlag(ItemSendsScenePositionChanges);
}

Pin::~Pin()
{
}

void Pin::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QGraphicsRectItem::contextMenuEvent(event);
}

void Pin::mousePressEvent(QGraphicsSceneMouseEvent *event)//pin clicked
{
    if(event->button() == Qt::LeftButton)
    {
        if(dynamic_cast<Component*>(this->parentItem()->parentItem())){
            Component* a = dynamic_cast<Component*>(this->parentItem()->parentItem());
            emit a->pin_hover_signal(this);
        }else if(dynamic_cast<Port*>(this->parentItem())){
            Port* a = dynamic_cast<Port*>(this->parentItem());
            emit a->pin_hover_signal(this);
        }
    }
    QGraphicsRectItem::mousePressEvent(event);
}

void Pin::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    this->setBrush(QBrush(Qt::red));
    QGraphicsRectItem::hoverEnterEvent(event);
}

void Pin::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    this->setBrush(QBrush(Qt::black));
    QGraphicsRectItem::hoverLeaveEvent(event);
}

void Pin::add_line(QGraphicsItem * line)
{
    this->vec_of_connections.push_back(line);
}

void Pin::remove_line(QGraphicsItem * line)
{
    vec_of_connections.removeAll(line);
}

QVariant Pin::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(this->parentItem())
    this->parentItem()->update(this->parentItem()->boundingRect());

    if(change == QGraphicsItem::ItemScenePositionHasChanged){

        for (auto i : vec_of_connections){
            if(auto j = dynamic_cast<FlatLine*>(i)){
                if(j->start == this){
                    j->setLine(value.toPointF().x(), value.toPointF().y(), j->end->scenePos().x(), j->end->scenePos().y());
                }
                if(j->end == this){
                    j->setLine(j->start->scenePos().x(), j->start->scenePos().y(), value.toPointF().x(), value.toPoint().y());

                }
            }
            if(auto j = dynamic_cast<OrtogonalLine*>(i)){
                if(j->start == this){
                    j->setLine(value.toPointF().x(), value.toPointF().y(), j->end->scenePos().x(), j->end->scenePos().y());
                }
                if(j->end == this){
                    j->setLine(j->start->scenePos().x(), j->start->scenePos().y(), value.toPointF().x(), value.toPoint().y());

                }
            }

        }
    }

    return QGraphicsRectItem::itemChange(change,value);
}



//---------------------------- Pixmap------------------------------

Component::M_pixmap::M_pixmap(Component *ptr)
{
    this->parentptr = ptr;
}

Component::M_pixmap::~M_pixmap()
{
}

void Component::M_pixmap::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{

    QMenu * menu = new QMenu;
    QAction * act1 = menu->addAction("rotate");
    QAction * act2 =  menu->addAction("remove");
    QAction * act3 =  menu->addAction("properties");

    QWidget * widget = new QWidget;
    widget->move(event->screenPos().rx(),event->screenPos().ry());

    connect(act1, &QAction::triggered, [this](){
        QTransform txf = QTransform();
        qreal a = this->rotation() + 90;
        txf.translate((12+12),(0));
        txf.rotate(a, Qt::ZAxis);
        txf.translate(0,0);
        this->setTransform(txf, true);


        if(parentptr->property("rotation_angle").toInt() != 270)
            parentptr->setProperty("rotation_angle", parentptr->property("rotation_angle").toInt() + 90);
        else{
            parentptr->setProperty("rotation_angle", 0);
        }
    });
    connect(act2, &QAction::triggered, [this](){
        //this->scene()->removeItem(this->parentItem());
        this->parentptr->~Component();
    });
    connect(act3, &QAction::triggered, [this, act1](){
        QDialog* contextmenu_dialog = new QDialog;
        contextmenu_dialog->deleteLater();
        QGridLayout* contextmenu_dialog_layout = new QGridLayout(contextmenu_dialog);
        contextmenu_dialog->setWindowTitle(parentptr->objectName() + " properties");

        QTableWidget* tablewidget1 = new QTableWidget(contextmenu_dialog);
        tablewidget1->setColumnCount(3);
        tablewidget1->setHorizontalHeaderLabels({"name", "value", "state"});
        tablewidget1->setRowCount(parentptr->dynamicPropertyNames().size());

        connect(tablewidget1, &QTableWidget::cellChanged, [this, tablewidget1, act1](int row, int column){
            qDebug() << parentptr->dynamicPropertyNames().at(column - 1);
            qDebug() << tablewidget1->item(row,column)->data(Qt::DisplayRole);
            if(parentptr->dynamicPropertyNames().at(column - 1) == "rotation_angle"){
                act1->trigger();
            }
            parentptr->setProperty(parentptr->dynamicPropertyNames().at(column - 1), tablewidget1->item(row,column)->data(Qt::DisplayRole));
        });
        tablewidget1->blockSignals(true);
        for( int i = 0; i < parentptr->dynamicPropertyNames().size(); ++i){
            QTableWidgetItem* nameitem1 = new QTableWidgetItem();
            nameitem1->setData(Qt::ItemDataRole::DisplayRole,
                               parentptr->dynamicPropertyNames().at(i));
            tablewidget1->setItem(i, 0, nameitem1);

            QTableWidgetItem* valueitem1 = new QTableWidgetItem();
            valueitem1->setData(Qt::ItemDataRole::DisplayRole,
                             parentptr->property( parentptr->dynamicPropertyNames().at(i)));
            tablewidget1->setItem(i, 1, valueitem1);

            if(parentptr->tri_states_map.contains(parentptr->dynamicPropertyNames().at(i) + "_tri_state")){

                QString prop_states = parentptr->tri_states_map[
                                      parentptr->dynamicPropertyNames().at(i) + "_tri_state"];
                QComboBox* comboitem = new QComboBox(contextmenu_dialog);
                QStringList state_list = prop_states.split(",");

                for(auto i : state_list){
                    comboitem->addItem(i);
                }
                comboitem->setCurrentText(parentptr->tri_states_map[
                                          parentptr->dynamicPropertyNames().at(i) + "_tri_state_active"]);
                tablewidget1->setCellWidget(i, 2, comboitem);

                connect(comboitem, QOverload<const QString &>::of(&QComboBox::currentIndexChanged), [this, i, comboitem, tablewidget1](const QString &text){
                    QString old_text = parentptr->tri_states_map[parentptr->dynamicPropertyNames().at(i) + "_tri_state_active"];
                    parentptr->tri_states_map.remove(parentptr->dynamicPropertyNames().at(i) + "_tri_state_active");
                    parentptr->tri_states_map.insert(parentptr->dynamicPropertyNames().at(i) + "_tri_state_active", text);
                    comboitem->currentIndex();

                    int old_index = comboitem->findText(old_text);
                    int new_index = comboitem->currentIndex();
                    if(old_index > new_index){
                        parentptr->setProperty(parentptr->dynamicPropertyNames().at(i), parentptr->property(parentptr->dynamicPropertyNames().at(i)).toDouble() / 1000);
                        tablewidget1->item(i, 1)->setData(Qt::DisplayRole, tablewidget1->item(i, 1)->data(Qt::DisplayRole).toDouble() / 1000);
                    }
                    if(old_index < new_index){
                        parentptr->setProperty(parentptr->dynamicPropertyNames().at(i), parentptr->property(parentptr->dynamicPropertyNames().at(i)).toDouble() * 1000);
                        tablewidget1->item(i, 1)->setData(Qt::DisplayRole, tablewidget1->item(i, 1)->data(Qt::DisplayRole).toDouble() * 1000);
                    }
                });
            }

        }
        tablewidget1->blockSignals(false);

    //    tablewidget1->setGeometry(0,0, 600, 400);
        contextmenu_dialog_layout->addWidget(tablewidget1);
        contextmenu_dialog->setLayout(contextmenu_dialog_layout);

    //    contextmenu_dialog_layout->setGeometry(QRect(0,0, 600,400));
        contextmenu_dialog->resize(350, 180);
        contextmenu_dialog->exec();

    });

    menu->exec(event->screenPos());

//    qDebug() << this->parentptr;
//    qDebug() << this->parentObject();



    QGraphicsPixmapItem::contextMenuEvent(event);
}


void Component::M_pixmap::setPixmap(const QString &ico)
{
    _icon_path = ico;
    QGraphicsPixmapItem::setPixmap(QPixmap(ico));
}

void Component::M_pixmap::setParent(Component *parent)
{
    this->parentptr = parent;
}

QString Component::M_pixmap::iconPath()
{
    return _icon_path;
}
