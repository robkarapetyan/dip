#ifndef M_VIEW_H
#define M_VIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QMenu>
#include <QContextMenuEvent>

class M_view : public QGraphicsView
{
Q_OBJECT
public:
    explicit M_view(QWidget *parent = nullptr);
    ~M_view();
    void contextMenuEvent(QContextMenuEvent *event);
signals:
    void rotate_sig(int angle);

public slots:
    void do_stuff();
    void received_options(QGraphicsItem *,QAction *);
};




#endif // M_VIEW_H
