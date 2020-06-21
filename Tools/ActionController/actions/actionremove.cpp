#include "actionremove.h"

ActionRemove::ActionRemove(Component *object, QGraphicsView *view): M_Action(view)
{
    _type = Type::remove;
    _object = object;
}

void ActionRemove::apply()
{
    _view->scene()->removeItem(_object);
    if(!_object->vec_of_pins.empty()){
        for(auto i : _object->vec_of_pins){
            if(!i->vec_of_connections.empty()){
                for( auto j : i->vec_of_connections){
                    _view->scene()->removeItem(j);
                }
            }
        }
    }
}

