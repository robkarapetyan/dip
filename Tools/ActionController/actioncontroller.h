#ifndef ACTIONCONTROLLER_H
#define ACTIONCONTROLLER_H

#include "actions/act.h"
#include "common_files/cyclicchache.h"


class ActionController
{
    CyclicChache* _chache = nullptr;
public:
    ActionController();
    void Do(M_Action* act);
    void undo();
    void redo();
private:
    int redo_counter = 0;
};

#endif // ACTIONCONTROLLER_H
