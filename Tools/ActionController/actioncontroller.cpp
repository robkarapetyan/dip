#include "actioncontroller.h"

ActionController::ActionController()
{
    _chache = new CyclicChache(5);
}

void ActionController::Do(M_Action *act)
{
    //apply and add action to queue
    _chache->insert(act);
    _chache->_current->previous->action()->apply();
//    qDebug() << (_chache->_current->previous->action()->type() == Type::remove);
    Node* cr = _chache->root;

    for(int i = 0; i < 5; ++i){
        qDebug() << i;

        if(cr->action())
        qDebug() << cr->action()->_object->objectName();
        else
        qDebug() << cr->action();
//        if(cr->next)
        cr = cr->next;
    }
    redo_counter = 0;
}

void ActionController::undo()
{
    //apply previous action's opposite action ( move current backward )
    //redo counter + 1 until < max_count
    if(!_chache->_current->previous)
        return;
    _chache->_current = _chache->_current->previous;

    if(!_chache->_current->action())
        return;
    _chache->_current->opposite()->apply();
    if(redo_counter <= _chache->_size)
    ++redo_counter;
}

void ActionController::redo()
{
    //if redo counter != 0, apply current action ( move current forward )
    // -- redo counter
    if(!_chache->_current || redo_counter == 0 || !_chache->_current->action())
        return;
    _chache->_current->action()->apply();
    _chache->_current = _chache->_current->next;
    --redo_counter;
}
