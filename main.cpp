#include "mainwindow.h"
#include <QApplication>

#include "Components/library.h"
#include "Components/resistor.h"
#include "Components/capacitor.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();

    return a.exec();
}
