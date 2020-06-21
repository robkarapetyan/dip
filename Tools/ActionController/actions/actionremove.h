#ifndef ACTIONREMOVE_H
#define ACTIONREMOVE_H

#include "Tools/ActionController/actions/act.h"

class ActionRemove : public M_Action
{
public:
    ActionRemove(Component* object, QGraphicsView* view);
    void apply() override;
};

#endif // ACTIONREMOVE_H
