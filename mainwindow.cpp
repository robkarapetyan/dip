#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWidget>
#include <QGraphicsPixmapItem>
#include <QLineEdit>
#include <QStringRef>
#include <QSpinBox>
#include "Components/resistor.h"
#include "Components/tools/component.h"
#include "Components/capacitor.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("test");
    connect(ui->graphicsView, SIGNAL(scaling_sig(int)), ui->spinBox,SLOT( setValue(int)));
    connect(ui->spinBox, SIGNAL(valueChanged(int)), ui->graphicsView,SLOT( change_scale(int)));

    //connect(this->ui->spinBox, SIGNAL(valueChanged(int)), ui->graphicsView, SLOT( ))
    ui->spinBox->setSuffix("%");
    ui->spinBox->setRange(40, 160);
    ui->spinBox->setValue(100);
    ui->spinBox->setSingleStep(10);

    ui->spinBox->setCorrectionMode(QSpinBox::CorrectionMode(QAbstractSpinBox::CorrectToNearestValue));

    QGraphicsScene* scene = new QGraphicsScene(this);
    //Custom_scene* scene = new Custom_scene(ui->graphicsView);
    QBrush brush(Qt::white);
    QGraphicsRectItem* rct = new QGraphicsRectItem(0,0,750,505);
    rct->hide();
    scene->addItem(rct);
    //scene->set

    //scene->setSceneRect(rec);
    scene->setBackgroundBrush(brush);
    //scene->setSceneRect(0,0,200,200);


    Resistor * res = new Resistor;
    //res->rotate(90);
    res->setPos(0,0);


    //scene->addItem(res);
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

                   // switching state of (graphicsView->comp) to (Components::capacitor)
                }
                else if(action->text() == "Inductor")
                {

                   // switching state of (graphicsView->comp) to (Components::inductor);
                }
                else if(action->text() == "Diode")
                {
                   // switching state of (graphicsView->comp) to (Components::diode);
                }
                else {
                   // switching state of (graphicsView->comp) to (Components::none);
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
