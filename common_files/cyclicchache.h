#ifndef CYCLICCHACHE_H
#define CYCLICCHACHE_H

#include "Tools/ActionController/actions/act.h"
#include "Tools/ActionController/actions/actionadd.h"
#include "Tools/ActionController/actions/actionremove.h"

struct Node{

    Node* next = nullptr;
    Node* previous = nullptr;

    Node(M_Action* act, Node* nxt,
         Node* prev);
    void clear();
    bool obj_in_scene = false;

    M_Action* opposite();
    M_Action* action();

    void set_action(M_Action* act);

private:
    M_Action* _opposite = nullptr;
    M_Action* _action = nullptr;
};



class CyclicChache
{
public:
    CyclicChache(const int& size);
    void insert(M_Action*);

    Node* root = nullptr;
    Node* _current = nullptr;

    int _size = 0;
};

#endif // CYCLICCHACHE_H
