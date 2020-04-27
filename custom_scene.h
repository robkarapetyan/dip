#ifndef CUSTOMSCENE_H
#define CUSTOM_SCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>

class Custom_scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Custom_scene(QObject *parent = 0);
    ~Custom_scene();

public slots:

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // CUSTOM_SCENE_H
