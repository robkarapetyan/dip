#ifndef CONNECTION_CONTROLLER_H
#define CONNECTION_CONTROLLER_H

#include <QWidget>
#include "Components/tools/component.h"
#include "ConnectionController/lines/iline.h"
#include "ConnectionController/lines/flatline.h"
#include "ConnectionController/lines/ortogonalline.h"

enum class ConnectionMode{ none, flat, polyline};

class Connection_controller : public QObject
{
    Q_OBJECT
public:
    Connection_controller(QObject *);
    ~Connection_controller() override;

    //to add already created connection from undo/redo stack
    void add_item(ILine*);
//    void remove_item(ILine*);
//    void delete_item(ILine*);

    void setMode(const ConnectionMode &a);
public slots:
    void receiving_pin(Pin*);
signals:
    void item_created(QGraphicsItem*);
private:
    QVector<Pin*> ports = {};
    ConnectionMode mode= ConnectionMode::none;
    QVector<ILine*> vec_of_lines = {};
    Pin* pin1 = nullptr;
    Pin* pin2 = nullptr;
};

#endif // CONNECTION_CONTROLLER_H
