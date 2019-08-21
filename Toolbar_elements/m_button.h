#ifndef M_BUTTON_H
#define M_BUTTON_H
#include <QPushButton>
#include <QMenu>
#include <QLineEdit>
#include <QWidgetAction>

class m_button : public QPushButton
{


public:
    QLineEdit * line = new QLineEdit;
    QMenu* m_menu = new QMenu;
    m_button();

    QMenu* Actions();
    void add_action(const char* str);
    void add_action(QAction& act);
    void add_action(const QPixmap &pix,const char* str);

};

#endif // M_BUTTON_H
