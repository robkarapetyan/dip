#include "cyclicchache.h"

CyclicChache::CyclicChache(const int& size)
{
    root = new Node(nullptr, nullptr, nullptr);
    Node* current = root;

    for(int i = 0; i < size - 1; ++i){
        current->next = new Node(nullptr, nullptr, nullptr);
        current->next->previous = current;
        current = current->next;

    }
    current->next = root;
    root->previous = current;
    _current = root;
    _size = size;
}

void CyclicChache::insert(M_Action* act)
{
    _current->clear();
    _current->set_action(act);
    _current = _current->next;
}

Node::Node(M_Action *act, Node *nxt, Node *prev) : next(nxt), previous(prev), _action(act){

}

void Node::clear()
{
    if(!_action)
        return;
    if(!obj_in_scene){
        delete _action->_object;
    }
    delete _action;
}

M_Action* Node::opposite()
{
    if(!_action)
        return nullptr;
    if(_action->type() == Type::add){
        obj_in_scene = false;
    }
    if(_action->type() == Type::remove){
        obj_in_scene = true;
    }
    return _opposite;
}

M_Action* Node::action()
{
    if(!_action)
        return nullptr;
    if(_action->type() == Type::add){
        obj_in_scene = true;
    }
    if(_action->type() == Type::remove){
        obj_in_scene = false;
    }
    return _action;
}

void Node::set_action(M_Action *act)
{
    if(!act)
        return;
    _action = act;
    if(act->type() == Type::add){
        _opposite = new ActionRemove(act->_object, act->_view);
    }
    if(act->type() == Type::remove){
        _opposite = new ActionAdd(act->_object, act->_view);
    }
}
