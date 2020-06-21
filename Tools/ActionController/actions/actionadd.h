#ifndef ACTIONADD_H
#define ACTIONADD_H

#include "act.h"


class ActionAdd : public M_Action
{
public:
    ActionAdd(Component* object, QGraphicsView* view);
    void apply() override;
};

#endif // ACTIONADD_H
