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
    ~M_view() override;

    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    Components comp = Components::none;
    std::vector<Component*> vec_of_components_p = {};
signals:
    void scaling_sig(int new_val);

public slots:
    void change_scale(int new_scale);
    void resistor_received();
    void capacitor_received();
private:
    int m_scaling = 100;
    QVector<QPointF> pvec = {};
};
#endif // M_VIEW_H
