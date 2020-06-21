#include "act.h"

M_Action::M_Action(QGraphicsView* view) : _view(view)
{

}

M_Action::~M_Action()
{

}

Type M_Action::type()
{
    return _type;
}
