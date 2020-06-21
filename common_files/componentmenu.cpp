#include "componentmenu.h"
#include <QDebug>

ComponentMenu::ComponentMenu(QWidget *parent) : QMenu(parent)
{
}

QAction* ComponentMenu::insert(const QString &new_header, const QString &parent_header)
{
    if(new_header == "")
        return nullptr;
    //find QAction named "parent_header" in this

    if(parent_header == "" || parent_header == "none"){
        return addAction(new_header);
    }

    if(QAction* a = find_action(this, parent_header)){
        if(!a->menu()){
            QMenu* mn = new QMenu(this);
            a->setMenu(mn);
        }
        return a->menu()->addAction(new_header);
    }

    //if QAction's menu is this, this->addaction(...)
    //else new QMenu && Qaction->setAction Qmenu->menuaction


    return nullptr;
}

QAction *ComponentMenu::find_action(QMenu *menu, const QString &headerName)
{
    if(!menu || menu->actions().isEmpty() || headerName == "")
        return nullptr;
    for(auto i : menu->actions()){
        if(i->text() == headerName){
            return i;
        }
    }

    for(auto i : menu->actions()){
        QAction* found = nullptr;
        if(i->menu()){
            found = find_action(i->menu(), headerName);
        }
        if(found)
            return found;
    }
    return  nullptr;
}
