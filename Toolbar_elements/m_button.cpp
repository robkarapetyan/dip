#include "m_button.h"

m_button::m_button()
{
    QWidgetAction * qwdact = new QWidgetAction(this);
//    m_area = new QScrollArea;
//    m_menu->setStyleSheet("QMenu { menu-scrollable: 1; }");

//    connect(m_menu, &QMenu::aboutToShow, [=](){
//        m_area->setWidget(m_menu);
//        m_area->show();
//    });

//    connect(m_menu, &QMenu::aboutToHide, [=](){
//        m_area->setWidget(m_menu);
//        m_area->hide();
//    });
//    m_menu->setMaximumHeight(300);
//    m_menu->setMaximumWidth(170);

    QObject::connect(line,SIGNAL(textChanged(QString)),this,SLOT(text_changed_is(QString)));
    QObject::connect(m_menu, SIGNAL( triggered(QAction*)), SLOT( m_menu_action_emmited(QAction *)));

    line->setPlaceholderText("Search");
    qwdact->setObjectName("search_action");
    qwdact->setDefaultWidget(line);
    setText("Elements");
    m_menu->addAction(qwdact);
    m_menu->setGeometry(0,0,10,10);

    this->setMenu(m_menu);
}

QMenu *m_button::Actions()
{
    return m_menu;
}

void m_button::add_action(const char* str)
{
    this->m_menu->addAction(str);
}

void m_button::add_action(QAction &act)
{
    this->m_menu->addAction(&act);
}

void m_button::add_action(const QPixmap &pix, const char *str)
{
    this->m_menu->addAction(pix,str);
}

QString m_button::lineedit_current_text()
{
    return line->text();
}

void m_button::update_actions(Library *lib)
{
    if(!lib)
    return;

    if(lib->headers().keys().isEmpty())
    return;

    for (auto i : menu()->actions()){
        if(i->objectName() != "search_action"){
            menu()->removeAction(i);
        }
    }

    QStringList namelist = lib->components("all");

    Component * current = nullptr;
    QAction* act = nullptr;
    for(auto i : namelist){
        current = lib->lib_has(i);
        act = new QAction(this);

        act->setIcon(QIcon(current->icon_path()));
//        qDebug() << current->icon_path();
        act->setIconVisibleInMenu(true);
        act->setText(i);
        menu()->addAction(act);
    }


    /*int previous = -1;
    for(auto i : lib->headers().keys()){
        if(i != previous && !lib->headers().keys().isEmpty()){
            //list of pairs with header(name, parent)
            QList<QPair<QString, QString> > valuelist = lib->headers().values(i);
            for(auto j : valuelist){
                QAction* headerAction = new QAction;
                headerAction->setObjectName(j.first);
                if(i == 0){
                    menu()->addAction(headerAction);
                }
                else{
//                    QList<QAction*> clist = menu()->actions()j.second, Qt::MatchContains|Qt::MatchRecursive, 0);
//                    if(!clist.empty()){
//                        clist.at(0)->addChild(headeritem);
//                    }
                }
            }
        }

        previous = i;
    }*/

}

void m_button::text_changed_is(QString text)
{
    foreach (QAction *action, this->menu()->actions())
    {
        if (!action->isSeparator() && !action->isWidgetType() &&  action != this->menu()->actions().at(0) )
        {
            if(text.size() <= action->text())
            {
                QString strin = action->text();
                QStringRef sref(&strin,0,text.size());

                if(sref == text)
                {
                    action->setVisible(true);
                }
                else{
                    action->setVisible(false);
                }
            }
        }
    }
}

void m_button::m_menu_action_emmited(QAction *act)
{
    emit action_triggered(act);
}
