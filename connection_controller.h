#ifndef CONNECTION_CONTROLLER_H
#define CONNECTION_CONTROLLER_H

#include <QWidget>
#include "Components/tools/component.h"
enum class ConnectionMode{ flat, polyline, none};

class Connection_controller : public QObject
{
    Q_OBJECT
public:
    Connection_controller();
    ~Connection_controller() override;

    void setMode(ConnectionMode a);
public slots:
    void receiving_pin(Pin*);
private:
    QVector<Pin*> ports = {};
    ConnectionMode mode= ConnectionMode::none;
};

#endif // CONNECTION_CONTROLLER_H
