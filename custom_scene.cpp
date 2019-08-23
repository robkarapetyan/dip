#include "custom_scene.h"
#include "Components/resistor.h"
#include "Components/capacitor.h"
#include "Components/component.h"
#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QInputDialog>
#include <QDir>


Custom_scene::Custom_scene(QObject *parent) :
    QGraphicsScene(parent)
{

}

Custom_scene::~Custom_scene()
{

}

void Custom_scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if(itemAt(event->pos().rx(),event->pos().ry(),QTransform()))
    {
        QList<QGraphicsItem *>  a = this->items(); // trying to access items this way,works >> causes type loss
        qDebug("%d", a.size());
            foreach(QGraphicsItem * item,a)
            {
            }
}
    QGraphicsScene::mousePressEvent(event);
}

void Custom_scene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{

QPointF p=event->screenPos();
    QGraphicsItem* item = this->itemAt(event->scenePos().rx(),event->scenePos().ry(),QTransform());
    if (item)
    {
            QMenu * menu = new QMenu;
            QAction * act1 = menu->addAction("rotate");
            QAction * act2 =  menu->addAction("properties");
            menu->exec(event->screenPos());

         if (qgraphicsitem_cast<Resistor *>(item) && !qgraphicsitem_cast<QGraphicsRectItem *>(item))
         {
             Resistor* res = qgraphicsitem_cast<Resistor *>(item);

             connect(act1, &QAction::triggered,menu, [act2,res](){

                 act2->setEnabled(false);

                 //not working also
                 /*
                 QTransform txf = QTransform();
                 qreal a = 0;
                 a = res->rotation() + 90;
                 txf.translate((12+12),(0));
                 txf.rotate(a, Qt::ZAxis);
                 txf.translate(0,0);
                 res->setTransform(txf, true);*/

             });

             connect(act2, &QAction::triggered,menu, [act1](){
                 act1->setEnabled(false);
             });

              if(act1->isEnabled())
              {
                  QTransform txf = QTransform();
                  qreal a = 0;
                  a = res->rotation() + 90;
                  txf.translate((12+12),(0));
                  txf.rotate(a, Qt::ZAxis);
                  txf.translate(0,0);
                  res->setTransform(txf, true);
              }

              if(act2->isEnabled())
              {

               bool ok;
                 int i = QInputDialog::getInt(menu, tr("Set Resistance"),
                                                              tr("resistance"), QLineEdit::Normal,
                                                              0,2000000,1, &ok);
                 if (ok)
                 {
                     res->resistance = i; // i - result taken from inputdialog
                 }
              }
        }
    }
    else {
        QMenu menu;
        menu.addAction("no component");
        menu.addAction("!!!!!!!!!!!!");
        menu.exec(event->screenPos());

        QGraphicsScene::contextMenuEvent(event);
    }
}
