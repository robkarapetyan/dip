#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWidget>
#include <QGraphicsPixmapItem>
#include <QLineEdit>
#include <QStringRef>
#include "Components/resistor.h"
#include "Components/tools/component.h"
#include "Components/capacitor.h"
#include "custom_scene.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("test");

    QGraphicsScene* scene = new QGraphicsScene(this);
    //Custom_scene* scene = new Custom_scene(ui->graphicsView);
    QBrush brush(Qt::white);
    QRectF rec(0,0,700,500);


    //scene->setSceneRect(rec);
    scene->setBackgroundBrush(brush);
    //scene->setSceneRect(0,0,200,200);


    Resistor * res = new Resistor;
    //res->rotate(90);
    res->setPos(0,0);


    scene->addItem(res);

    Capacitor * cap = new Capacitor;
    cap->setPos(550,600);
    scene->addItem(cap);

    //ui->graphicsView->vec_of_components_p.push_back(cap);

    ui->graphicsView->setScene(scene);
    //ui->graphicsView->setFrameShape(QGraphicsView::NoFrame);
    //ui->graphicsView->adjustSize();
    //ui->graphicsView->setContentsMargins(0,0,100,110);
    //scene->setForegroundBrush(QBrush(Qt::gray));
    //ui->graphicsView->setFixedSize(700, 800);
    //ui->graphicsView->scene()->setSceneRect(0, 0, 100, 200);

    //restmp->setPos(this->mapToScene(this->viewport()->rect().x(), this->viewport()->rect().y())
    //scene->setSceneRect(ui->graphicsView->viewport()->rect().x(), ui->graphicsView->viewport()->rect().y(),100,100);
    scene->setMinimumRenderSize(0.5);

    //ui->graphicsView->centerOn(0,0);

    //qDebug(" xz %i" ,ui->graphicsView->mapToScene(ui->graphicsView->viewport()->geometry()).boundingRect());

    //ui->graphicsView->mapFromScene(scene->itemsBoundingRect());

    //ui->graphicsView->viewport()->show();
    //ui->graphicsView->mapFrom(this,QPoint(0,0));

    //ui->graphicsView->mapToScene(QPoint(0,0));
    //ui->graphicsView->setBackgroundBrush(QBrush(Qt::black));

    but->setText("Elements");
    but->add_action(QPixmap("C:/Users/Rob/Documents/diplom_beginning/icons/resh.ico"),"Resistor");
    but->add_action(QPixmap("C:/Users/Rob/Documents/diplom_beginning/icons/caph.png"),"Capacitor");
    but->add_action("Inductor");
    but->add_action(QPixmap("C:/Users/Rob/Documents/diplom_beginning/icons/dioh.png"),"Diode");
    but->add_action("Zener");
    but->add_action("Capacitar");
    but->add_action("Capacitarer");
    but->add_action("Stabilitrone");

    foreach (QAction *action, but->m_menu->actions())
    {
            if (!action->isSeparator() && !action->isWidgetType() &&  action != but->m_menu->actions().at(0) )
            {
                if(action->text() == "Resistor")
                {
                    connect(action, SIGNAL(triggered()), ui->graphicsView,SLOT( resistor_received()));

                }
                else if(action->text() == "Capacitor")
                {
                    connect(action, SIGNAL(triggered()), ui->graphicsView,SLOT( capacitor_received()));

                   //ui->graphicsView->comp = Components::capacitor;
                }
                else if(action->text() == "Inductor")
                {

                   //ui->graphicsView->comp = Components::inductor;
                }
                else if(action->text() == "Diode")
                {
                   //ui->graphicsView->comp = Components::diode;
                }
                else {
                   // ui->graphicsView->comp = Components::none;
                }
            }
     }


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
