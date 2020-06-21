#include "actionadd.h"
#include "ConnectionController/lines/iline.h"

ActionAdd::ActionAdd(Component *object, QGraphicsView *view): M_Action(view)
{
    _type = Type::add;
    _object = object;
}

void ActionAdd::apply()
{
//    qDebug() << "add_apply";
    _view->scene()->addItem(_object);

    if(!_object->vec_of_pins.empty()){
        for(auto i : _object->vec_of_pins){
            if(!i->vec_of_connections.empty()){
                for( auto j : i->vec_of_connections){
                    ILine* linetmp = dynamic_cast<ILine*>(j);
                    if(_view->scene()->items().contains(linetmp->end) &&
                            _view->scene()->items().contains(linetmp->start))
                    _view->scene()->addItem(j);
                }
            }
        }
    }
}
