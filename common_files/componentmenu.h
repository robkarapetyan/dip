#ifndef COMPONENTMENU_H
#define COMPONENTMENU_H

#include <QMenu>

class ComponentMenu : public QMenu
{
    Q_OBJECT
public:
    ComponentMenu(QWidget *parent = nullptr);
    QAction* insert(const QString& new_header, const QString& parent_header);
//    QAction* find_action(const QString& header_name)
    QAction* a = nullptr;

private:
    QAction* find_action(QMenu* menu, const QString& headerName);
};

#endif // COMPONENTMENU_H
