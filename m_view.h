#ifndef M_VIEW_H
#define M_VIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QMenu>
#include <QContextMenuEvent>
#include "Components/capacitor.h"
#include "Components/resistor.h"
#include "Components/tools/component.h"

enum class Components{none,resistor,capacitor,inductor,diode};

class M_view : public QGraphicsView
{
Q_OBJECT
public:
    explicit M_view(QWidget *parent = nullptr);
    ~M_view();

    //void contextMenuEvent(QContextMenuEvent *event);
    void mousePressEvent(QMouseEvent* event);

    Components comp = Components::none;
    std::vector<Component*> vec_of_components_p = {};
signals:
    void rotate_sig(int angle);

public slots:

    void resistor_received();
    void capacitor_received();
    void received_options(QGraphicsItem *,QAction *);
};
#endif // M_VIEW_H
