#include "m_button.h"

m_button::m_button()
{
    QWidgetAction * qwdact = new QWidgetAction(this);

    QObject::connect(line,SIGNAL(textChanged(QString)),this,SLOT(text_changed_is(QString)));
    QObject::connect(m_menu, SIGNAL( triggered(QAction*)), SLOT( m_menu_action_emmited(QAction *)));

    line->setPlaceholderText("Search");
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
