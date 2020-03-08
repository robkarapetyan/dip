#include "connection_controller.h"
#include <QDebug>

Connection_controller::Connection_controller()
{

}
Connection_controller::~Connection_controller(){}

void Connection_controller::setMode(ConnectionMode a)
{
    mode = a;
}

void Connection_controller::receiving_pin(Pin *a)
{
//    qDebug() << "received it" << a <<"\n" << a->scenePos().x() << a->scenePos().y();

}
