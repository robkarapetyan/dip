#ifndef LINENODE_H
#define LINENODE_H

#include <QGraphicsItem>
#include "Components/tools/component.h"

class LineNode
{
public:
    LineNode();

    virtual ~LineNode();
private:
    QGraphicsRectItem* pin1 = nullptr;
    QGraphicsRectItem* pin2 = nullptr;
//    void update();
};

#endif // LINENODE_H
