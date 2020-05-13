#include "connection_controller.h"
#include <QMessageBox>
#include "mainwindow.h"


Connection_controller::Connection_controller(QObject *parent): QObject(parent)
{
    //meant to make and store connection instances
}
Connection_controller::~Connection_controller(){}

void Connection_controller::add_item(ILine * line)
{
    vec_of_lines.push_back(line);
    emit item_created(line);
}

//void Connection_controller::remove_item(ILine *)
//{

//}

//void Connection_controller::delete_item(ILine *)
//{

//}

void Connection_controller::setMode(const ConnectionMode& a)
{
    mode = a;
}

//receives pin's, makes and emits line instances
void Connection_controller::receiving_pin(Pin *a)
{
    try {
        if(mode != ConnectionMode::none){
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
                throw std::invalid_argument("cant attach pins of same parent");

            if(mode == ConnectionMode::flat){
                ILine* line = new FlatLine(pin1,pin2);

                line->setLine(pin1->scenePos().rx(),pin1->scenePos().ry(), pin2->scenePos().rx(), pin2->scenePos().ry());
                pin1->add_line(line);
                pin2->add_line(line);

                vec_of_lines.push_back(line);
                emit item_created(line);
            }else if(mode == ConnectionMode::polyline){
                ILine* line = new OrtogonalLine(pin1,pin2);

                line->setLine(pin1->scenePos().rx(),pin1->scenePos().ry(), pin2->scenePos().rx(), pin2->scenePos().ry());
                pin1->add_line(line);
                pin2->add_line(line);

                vec_of_lines.push_back(line);
                emit item_created(line);
            }

        }
    } catch (const std::invalid_argument& a) {
        QMessageBox* b = new QMessageBox;
        b->setIcon(QMessageBox::Critical);
        b->setText("Self attachment is not allowed");
        b->setWindowTitle("Error");
        b->setAttribute(Qt::WA_DeleteOnClose, true);
        b->addButton("Ok", QMessageBox::RejectRole);

        b->show();
        return;
    }

}
