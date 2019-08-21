#include "m_button.h"



m_button::m_button()
{
    QWidgetAction * qwdact = new QWidgetAction(this);


    line->setPlaceholderText("Search");
    qwdact->setDefaultWidget(line);
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
