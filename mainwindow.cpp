#include "mainwindow.h"
#include "custom_scene.h"
#include "ui_mainwindow.h"
#include <QWidget>
#include <QGraphicsPixmapItem>
#include <QLineEdit>
#include <QStringRef>
#include <QSpinBox>
#include <QGraphicsSvgItem>
#include <QFile>
#include <QDebug>
#include <QString>
#include <QFileDialog>
#include <QDomDocument>
#include "Components/resistor.h"
#include "Components/tools/component.h"
#include "Components/capacitor.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("test");
    connect(ui->graphicsView, SIGNAL( scaling_sig(int)), ui->spinBox,SLOT( setValue(int)));
    connect(ui->spinBox, SIGNAL( valueChanged(int)), ui->graphicsView,SLOT( change_scale(int)));

    //scale spinbox initialisation
    ui->spinBox->setCorrectionMode(QSpinBox::CorrectionMode
                                   (QAbstractSpinBox::CorrectToNearestValue));
    ui->spinBox->setValue(100);  //spinbox value is being set to range's minimum value(40) without this

    //scene initialisation
    Custom_scene* scene = new Custom_scene(this);    
    QGraphicsRectItem* rct = new QGraphicsRectItem(0,0,750,505);
    rct->hide();
    scene->addItem(rct);
    scene->setBackgroundBrush(QBrush(Qt::white));
    ui->graphicsView->setScene(scene);



    //library initialisation
    lib = new Library;
    lib->init();


    //Remove from here or initialisate with lib
    //toolbar searchbutton initialisation and setup  ======
    but = new m_button;
    connect(but, SIGNAL( action_triggered(QAction*)), this,
            SLOT( toolbar_button_action_receiver(QAction*)));
    but->menu()->setMaximumHeight(300);
    ui->mainToolBar->addWidget(but);
    but->update_actions(lib);

    //library usage example
//    lib->add_component(new Capacitor);
//    lib->add_component(new Resistor);
//    Resistor* res = new Resistor;
//    lib->add_component(res);
    connect(this, SIGNAL( current_component(Component*)), ui->graphicsView,
            SLOT(component_received(Component *)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSave_triggered()
{
    QString newPath = QFileDialog::getSaveFileName(this, "Save SVG",
            current_file, tr("SVG files (*.svg)"));

        if (newPath.isEmpty())
            return;
//        qDebug() << newPath;
//        qDebug() << current_file;
        current_file = newPath;

            QSvgGenerator generator;        // Create a file generator object
        generator.setFileName(current_file);    // We set the path to the file where to save vector graphics
        generator.setSize(QSize(ui->graphicsView->scene()->width() , ui->graphicsView->scene()->height() ));  // Set the dimensions of the working area of the document in millimeters
        generator.setViewBox(QRect(0, 0, ui->graphicsView->scene()->width(), ui->graphicsView->scene()->height() )); // Set the work area in the coordinates
        generator.setTitle("SVG Example");
        //generator.setDescription(trUtf8("File created by SVG Example"));

        QPainter painter;
        painter.begin(&generator);
        ui->graphicsView->scene()->render(&painter);
        painter.end();

        QDomDocument doc;       // document object
        QFile file(newPath);   // Open SVG-file
        doc.setContent(&file);

        QDomElement svg_tag = doc.elementsByTagName("svg").at(0).toElement();
//qDebug() << "size is " <<  doc.elementsByTagName("svg").size();
        QDomElement unparse_ = doc.createElement( "to_unparse" );
        svg_tag.appendChild(unparse_);

        QDomElement components_section = doc.createElement("components_section");
        QDomElement connection_manager_section = doc.createElement("cnnection_manager_section");

        unparse_.appendChild(components_section);
        unparse_.appendChild(connection_manager_section);
        ui->graphicsView->scene()->update();
        for (auto i : ui->graphicsView->scene()->items()){
                if(dynamic_cast<Component*>(i)){
                   Component* a = dynamic_cast<Component*>(i);
                   QDomElement component = doc.createElement("component");
                   component.setAttribute("type", a->objectName());
                   component.setAttribute( "x", a->scenePos().rx());
                   component.setAttribute( "y", a->scenePos().ry());
                   qDebug() << "x is " << a->scenePos().rx() << "y is " << ui->graphicsView->scene()->height() ;
//                   qDebug() << "x is " << QString("%1").arg(a->scenePos().x());
                   components_section.appendChild(component);
                }
        }
//        ui->graphicsView->scene()->


//            QDomElement resist = doc.createElement("component");
//            //resist.setPrefix("resistor");
//            resist.setAttribute( "x", 200 );
//            resist.setAttribute( "y", 300);

//            resist.setAttribute( "resistance", 100);


        QFile outFile(newPath);
        if( !outFile.open( QIODevice::WriteOnly | QIODevice::Text ) )
          {
            qDebug( "Failed to open file for writing." );
          }

        QTextStream stream( &outFile );
        stream << doc.toString();

        outFile.close();
}

void MainWindow::on_actionOpen_triggered()
{

    QString newPath = QFileDialog::getOpenFileName(this, "Open SVG",
                                                        current_file, tr("SVG files (*.svg)"));
    if (newPath.isEmpty())
        return;

    QDomDocument doc;       // document object
    QFile file(newPath);   // Open the SVG-file
    // If it is not opened, or have failed to transmit the contents in QDocDocument
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return;

    QDomElement svg_tag = doc.elementsByTagName("svg").at(0).toElement();

    QList<QString> viewDimention = svg_tag.attribute("viewBox").split(" ");

    QGraphicsRectItem* rect = new QGraphicsRectItem(0,0,viewDimention[2].toInt(),viewDimention[3].toInt());
    QGraphicsScene* newscene = new QGraphicsScene;
    delete ui->graphicsView->scene();
    ui->graphicsView->setScene(newscene);

    ui->graphicsView->scene()->addItem(rect);

    QDomNodeList list_of_components = doc.elementsByTagName("component");

    for(int i = 0;i < list_of_components.size(); ++i){
        if(list_of_components.at(i).toElement().attribute("type") == "resistor"){
            Resistor* resist = new Resistor;
            ui->graphicsView->scene()->addItem(resist);
            resist->moveBy(list_of_components.at(i).toElement().attribute("x").toDouble(), list_of_components.at(i).toElement().attribute("y").toDouble());
        }
        else if(list_of_components.at(i).toElement().attribute("type") == "capacitor"){
            Capacitor* cap = new Capacitor;
            ui->graphicsView->scene()->addItem(cap);
            cap->setPos(list_of_components.at(i).toElement().attribute("x").toDouble(), list_of_components.at(i).toElement().attribute("y").toDouble());
        }
    }

    file.close();
    ui->graphicsView->scene()->removeItem(rect);
    delete rect;
}

void MainWindow::on_actionadd_port_triggered()
{
    ui->graphicsView->set_to_(ActiveMode(ActiveMode::port));
}

void MainWindow::on_actionplain_connection_triggered()
{
    this->ui->graphicsView->conncontroller->setMode(ConnectionMode::flat);
    this->ui->graphicsView->set_to_(ActiveMode::none);
}

void MainWindow::on_actioncurved_connection_triggered()
{
    this->ui->graphicsView->conncontroller->setMode(ConnectionMode::polyline);
    this->ui->graphicsView->set_to_(ActiveMode::none);
}

void MainWindow::on_actionLibrary_Editor_triggered()
{
    if(!editor){
        editor = new LibEditor(this);
    }
    editor->setModal(true);
    editor->setLib(lib);
    editor->update_treewidget();
    if(editor->exec() == 1){
        lib->update();
        but->update_actions(lib);
    }
}

//void MainWindow::on_actionaction_grid_triggered()
//{
//    if(ui->actionaction_grid->isChecked()){
//        ui->graphicsView->setGrid_enabled(true);
//    }
//    else
//        ui->graphicsView->setGrid_enabled(false);

//}

void MainWindow::on_actionpassive_triggered()
{
//    update_component_menu();

    but->update_actions(lib);

}

void MainWindow::toolbar_button_action_receiver(QAction *act)
{
    if(lib->lib_has(act->text())){
        emit current_component(lib->lib_has(act->text()));
    }
}

QAction *MainWindow::find_action(QAction *, const QString&)
{
//    for( auto i : head->menu()->actions()){
//        if(i->objectName() == objname)
//            return i;
//    }

//    for( auto i : head->menu()->actions()){
//        if(i->menu() != head->menu()){
//            if(QAction* a = find_action(i, objname)){
//                    return a;
//            }
//        }
//    }

    return nullptr;
}

void MainWindow::update_component_menu()
{
    if(lib->headers().keys().isEmpty())
    return;
    //clear menu,set some new
    ui->menucomponents->clear();

//    //add headers as actions
//    int previous = -1;
//    for(auto i : lib->headers().keys()){
//        if(i != previous && !lib->headers().keys().isEmpty()){
//            //list of pairs with header(name, parent)
//            QList<QPair<QString, QString> > valuelist = lib->headers().values(i);
//            for(auto j : valuelist){
//                QAction* headerAction = new QAction;
//                headerAction->setObjectName(j.first);
//                if(i == 0){
//                    ui->menucomponents->addAction(headerAction);
//                }
//                else{
//                    QAction* parentaction = nullptr;
//                    for(auto i : ui->menucomponents->actions()){
//                        if(i->objectName() == j.second){
//                            parentaction = i;
//                            break;
//                        }
//                    }
//                    if(!parentaction){
//                        for(auto i : ui->menucomponents->actions()){
//                            parentaction = find_action(i, j.first);
//                            if(parentaction){
//                                break;
//                            }
//                        }
//                    }

//                    if(!parentaction){
//                        qDebug() << "not found -_-" << j.first;
//                    }

//                    QAction* newAction = new QAction;
//                    newAction->setObjectName(j.first);
//                    if(parentaction->menu()){
//                        parentaction->menu()->addAction(newAction);
//                    }else{
//                        parentaction->setMenu(new QMenu(parentaction->parentWidget()));
//                        parentaction->menu()->addAction(newAction);

//                    }

////                    QList<QAction*> clist = menu()->actions()j.second, Qt::MatchContains|Qt::MatchRecursive, 0);
////                    if(!clist.empty()){
////                        clist.at(0)->addChild(headeritem);
////                    }
//                }
//            }
//        }

//        previous = i;
//    }
}
