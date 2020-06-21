#ifndef ACTION_H
#define ACTION_H

#include <QGraphicsView>
#include "Components/tools/component.h"

//#include "Tools/ActionController/actioncontroller.h"

enum class Type{add, remove, move};

class M_Action
{
public:
    M_Action(QGraphicsView* view = nullptr);
    virtual ~M_Action();
    Type type();
    virtual void apply() = 0;
    QGraphicsView* _view = nullptr;
    Component* _object = nullptr;

protected:
    Type _type = Type::add;
};

#endif // ACTION_H
