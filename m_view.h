#ifndef M_VIEW_H
#define M_VIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QMenu>
#include <QContextMenuEvent>
#include <QGraphicsLineItem>
#include "Components/capacitor.h"
#include "Components/resistor.h"
#include "Components/tools/component.h"
#include "ConnectionController/connection_controller.h"

enum class ActiveMode{none,component,port};

class M_view : public QGraphicsView
{
Q_OBJECT
public:
    explicit M_view(QWidget *parent = nullptr);
    ~M_view() override;

    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    QStringList get_spice();
    ActiveMode mode = ActiveMode::none;
//    std::vector<Component*> vec_of_components_p = {};
    void set_to_(const ActiveMode& a);
    Connection_controller* conncontroller;
//    void setGrid_enabled(bool);

signals:
    void scaling_sig(int new_val);

public slots:
    void add_received_lineItem(QGraphicsItem*);
    void change_scale(int new_scale);
    void component_received(Component *);
private:
    int m_scaling = 100;
//    QGraphicsItemGroup* gridgroup = nullptr;
    Component* active_component = nullptr;
public:
    bool grabFlag = false;
};
#endif // M_VIEW_H
