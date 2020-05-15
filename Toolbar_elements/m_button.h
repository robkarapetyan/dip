#ifndef M_BUTTON_H
#define M_BUTTON_H
#include <QPushButton>
#include <QMenu>
#include <QLineEdit>
#include <QWidgetAction>
#include <QStringList>
#include <QScrollArea>
#include "Components/library.h"

class m_button : public QPushButton
{
    Q_OBJECT
public:
    m_button();

    QMenu* Actions();
    void add_action(const char* str);
    void add_action(QAction& act);
    void add_action(const QPixmap &pix,const char* str);
    QString lineedit_current_text();
    void update_actions(Library* lib);
public slots:
    void text_changed_is(QString);
    void m_menu_action_emmited(QAction*);
signals:
    void action_triggered(QAction*);
private:
    QLineEdit * line = new QLineEdit;
    QMenu* m_menu = new QMenu;
private:
    QScrollArea* m_area = nullptr;
};

#endif // M_BUTTON_H
