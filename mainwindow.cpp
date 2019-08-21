#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWidget>
#include <QGraphicsPixmapItem>
#include <QLineEdit>
#include <QStringRef>
#include "Components/resistor.h"
#include "Components/component.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("test");

    QGraphicsScene* scene = new QGraphicsScene;
    QBrush brush(Qt::white);
    QRectF rec(0,0,700,500);



    scene->setSceneRect(rec);
    scene->setBackgroundBrush(brush);

    Resistor * res = new Resistor;
    res->rotate(90);
    scene->addItem(res);



    ui->graphicsView->setScene(scene);

    but->setText("Elements");
    but->add_action(QPixmap("C:/Users/Rob/Documents/diplom_beginning/icons/resh.ico"),"Resistor");
    but->add_action(QPixmap("C:/Users/Rob/Documents/diplom_beginning/icons/caph.png"),"Capacitor");
    but->add_action("Inductor");
    but->add_action(QPixmap("C:/Users/Rob/Documents/diplom_beginning/icons/dioh.png"),"Diode");
    but->add_action("Zener");
    but->add_action("Capacitar");
    but->add_action("Capacitarer");
    but->add_action("Stabilitrone");



    QObject::connect(but->line,SIGNAL(textChanged(QString)),this,SLOT(slot1(QString)));
    ui->mainToolBar->addWidget(but);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot1(QString  text )
{
    foreach (QAction *action, but->m_menu->actions())
    {
            if (!action->isSeparator() && !action->isWidgetType() &&  action != but->m_menu->actions().at(0) )
            {
                if(text.size() <= action->text())
                {
                    QString strin = action->text();
                    QStringRef sref(&strin,0,text.size());

                    if(sref == text)
                    {
                        action->setVisible(true);
                    }
                    else{
                        action->setVisible(false);
                    }
                }

            }
     }

   // qDebug("%s",qUtf8Printable(text));
}
