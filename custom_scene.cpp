#include "custom_scene.h"
#include "Components/resistor.h"
#include "Components/capacitor.h"
#include "Components/tools/component.h"
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

}
    QGraphicsScene::mousePressEvent(event);
}

/*void Custom_scene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QGraphicsItem* item = this->itemAt(event->scenePos().rx(),event->scenePos().ry(),QTransform());
    if (item)
    {
            QMenu * menu = new QMenu;
            QAction * act1 = menu->addAction("rotate");
            QAction * act2 =  menu->addAction("properties");
            menu->exec(event->screenPos());

         if (qgraphicsitem_cast<Resistor *>(item) )
         {
             Resistor* res = qgraphicsitem_cast<Resistor *>(item);

             connect(act1, &QAction::triggered,this, [res](){

                 {
                     QTransform txf = QTransform();
                     qreal a = res->rotation() + 90;
                     txf.translate((12+12),(0));
                     txf.rotate(a, Qt::ZAxis);
                     txf.translate(0,0);
                     res->setTransform(txf, true);
                 }
             });
             connect(act2, &QAction::triggered,this, [act1,act2,menu,res](){
                 act1->setEnabled(false);
                 act2->setEnabled(true);
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
             });
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
*/
