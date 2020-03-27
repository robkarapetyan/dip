#include "connection_controller.h"
#include <QDebug>
#include "mainwindow.h"
#include <QMessageBox>


Connection_controller::Connection_controller(QObject *parent): QObject(parent)
{

}
Connection_controller::~Connection_controller(){}

void Connection_controller::setMode(const ConnectionMode& a)
{
    mode = a;
}

//void Connection_controller::setView(QGraphicsView *a)
//{
//    m_view = a;
//}

void Connection_controller::receiving_pin(Pin *a)
{
    try {
        qDebug() << a;
        if(mode == ConnectionMode::flat){
            if(!pin1){
                pin1 = a;
                return;
            }
            if(pin1 && pin2){
                pin1 = a;
                pin2 = nullptr;
                return;
            }
            if(pin1 && !pin2){
                pin2 = a;
            }

            if(pin1->parentItem() == pin2->parentItem())
                throw std::invalid_argument("cant attach pins of same parent with flat line");

            QGraphicsLineItem* line = new QGraphicsLineItem;

            line->setLine(pin1->scenePos().rx(),pin1->scenePos().ry(), pin2->scenePos().rx(), pin2->scenePos().ry());
    //        this->m_view->scene()->addItem(line);
            line->setFlag(QGraphicsLineItem::ItemIsMovable);
            emit item_created(line);
        }
    } catch (const std::invalid_argument& a) {
//        qDebug() << a.what();
        QMessageBox* b = new QMessageBox;
        b->setIcon(QMessageBox::Critical);
        b->setText("Self attachment with flat line not allowed");
        b->setWindowTitle("Error");
        b->setAttribute(Qt::WA_DeleteOnClose, true);
        b->addButton("Ok", QMessageBox::RejectRole);

        b->show();
        return;
    }


    qDebug() << "received it" << a->scenePos();
}
