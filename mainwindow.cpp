#include "mainwindow.h"
#include "custom_scene.h"
#include "ui_mainwindow.h"
#include <QWidget>
#include <QGraphicsPixmapItem>
#include <QLineEdit>
#include <QStringRef>
#include <QSpinBox>
#include <QGraphicsSvgItem>
#include <QColorDialog>
#include <QFile>
#include <QDebug>
#include <QString>
#include <QRadioButton>
#include <QFileDialog>
#include <QDomDocument>

#include <QProcess>
#include <WMIUtils.h>
#include <QFileInfo>

#include "Components/tools/port.h"
#include "Components/resistor.h"
#include "Components/tools/component.h"
#include "Components/capacitor.h"
#include "common_files/componentmenu.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(".");
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
    update_component_menu();
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
        ui->graphicsView->scene()->update();// ??
        for (auto i : ui->graphicsView->scene()->items()){
                if(dynamic_cast<Component*>(i)){
                   Component* cmp = dynamic_cast<Component*>(i);
                   QDomElement component = doc.createElement("component");

                   for (auto i :  cmp->dynamicPropertyNames()){

                       if(cmp->tri_states_map.find(i + "_tri_state_active") != cmp->tri_states_map.end()){
                           component.setAttribute(i, cmp->property(i).toString() + " "
                                                  + cmp->tri_states_map[i + "_tri_state_active"]);
                       }else{
                           component.setAttribute(i, cmp->property(i).toString());
                       }
                   }
                   component.setAttribute("type", cmp->objectName());
                   component.setAttribute( "x", cmp->scenePos().rx());
                   component.setAttribute( "y", cmp->scenePos().ry());
//                   qDebug() << "x is " << a->scenePos().rx() << "y is " << ui->graphicsView->scene()->height() ;
//                   qDebug() << "x is " << QString("%1").arg(a->scenePos().x());
                   components_section.appendChild(component);
                }
        }

        for (auto i : ui->graphicsView->conncontroller->get_lines()){
            QDomElement line = doc.createElement("line");
            QDomElement start = doc.createElement("start");
            QDomElement end = doc.createElement("end");

            if(dynamic_cast<FlatLine*>(i)){
                line.setAttribute("type", "flat");
            }else if (dynamic_cast<OrtogonalLine*>(i)){
                line.setAttribute("type", "orthogonal");
            }
            else if(!i){
                continue;
            }
            start.setAttribute("x", i->start->scenePos().rx());
            start.setAttribute("y", i->start->scenePos().ry());
            start.setAttribute("signature", i->start->getSignature());

            end.setAttribute("x", i->end->scenePos().rx());
            end.setAttribute("y", i->end->scenePos().ry());
            end.setAttribute("signature", i->end->getSignature());

            line.appendChild(start);
            line.appendChild(end);

            connection_manager_section.appendChild(line);
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
    ui->graphicsView->conncontroller->ports.clear();

    ui->graphicsView->names_map.clear();
    QString newPath = QFileDialog::getOpenFileName(this, "Open SVG",
                                                        current_file, tr("SVG files (*.svg)"));
    if (newPath.isEmpty())
        return;

    QDomDocument doc;       // document object
    QFile file(newPath);   // Open the SVG-file
    // If it is not opened, or have failed to transmit the contents in QDocDocument
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return;

    current_file = newPath;

    QDomElement svg_tag = doc.elementsByTagName("svg").at(0).toElement();

    QList<QString> viewDimention = svg_tag.attribute("viewBox").split(" ");

    QGraphicsRectItem* rect = new QGraphicsRectItem(0,0,viewDimention[2].toInt(),viewDimention[3].toInt());
    if(rect->rect().width() < 400){
        rect->setRect(0,0, 750,505);
    }
    QGraphicsScene* newscene = new QGraphicsScene;
    delete ui->graphicsView->scene();
    ui->graphicsView->setScene(newscene);

    ui->graphicsView->scene()->addItem(rect);

    QDomNodeList list_of_components = doc.elementsByTagName("component");
    QDomNodeList list_of_lines = doc.elementsByTagName("line");

    if(list_of_components.isEmpty() || list_of_lines.isEmpty()){
        return;
    }
    //adding components
    Component* cmp = nullptr;
    for(int i = 0; i < list_of_components.size(); ++i){
        cmp = nullptr;
        if( cmp = lib->lib_has(list_of_components.at(i).toElement().attribute("type"))){
            QDomElement bb = list_of_components.at(i).toElement();
            QDomNamedNodeMap map = bb.attributes();

            for(int j = 0; j < map.length(); ++j){
//                qDebug() << "heheheheyy .. " <<  bb.attributes().item(j).toAttr().name().toUtf8() << " : " << bb.attributes().item(j).toAttr().value().toUtf8();
                if(map.item(j).toAttr().name().toUtf8() != "spice_form" &&
                        map.item(j).toAttr().name().toUtf8() != "x" &&
                        map.item(j).toAttr().name().toUtf8() != "y")
                {

                    if(cmp->tri_states_map.contains(map.item(j).toAttr().name().toUtf8()
                                                    + "_tri_state_active")){
                        QString multiprop = map.item(j).toAttr().value().toUtf8();

                        cmp->tri_states_map.insert(map.item(j).toAttr().name().toUtf8()
                                                   + "_tri_state_active", multiprop.split(" ").last());
                        cmp->setProperty(map.item(j).toAttr().name().toUtf8(),
                                         *multiprop.split(" ").begin());
                    }else{
                        cmp->setProperty(map.item(j).toAttr().name().toUtf8(),
                                         map.item(j).toAttr().value().toUtf8());
                    }
                }
            }
            ui->graphicsView->scene()->addItem(cmp);
            connect(cmp, SIGNAL(pin_hover_signal(Pin*)), ui->graphicsView->conncontroller, SLOT(receiving_pin(Pin *) ));
            cmp->setPos(list_of_components.at(i).toElement().attribute("x").toDouble(), list_of_components.at(i).toElement().attribute("y").toDouble());
        }
    }

    //adding connections
    for(int i = 0; i < list_of_lines.size(); ++i){
        if( list_of_lines.at(i).toElement().attribute("type").toUtf8() == "flat"){
            ui->graphicsView->conncontroller->setMode(ConnectionMode::flat);
        }else if(list_of_lines.at(i).toElement().attribute("type").toUtf8() == "orthogonal"){
            ui->graphicsView->conncontroller->setMode(ConnectionMode::polyline);
        }
        QDomNode startNode = list_of_lines.at(i).toElement().childNodes().at(0);
        QDomNode endNode = list_of_lines.at(i).toElement().childNodes().at(1);

        //start position
        QPointF startPos(startNode.toElement().attribute("x").toDouble(),
                startNode.toElement().attribute("y").toDouble());
        QString startSign = startNode.toElement().attribute("signature").toUtf8();

        //end position
        QPointF endPos(endNode.toElement().attribute("x").toDouble(),
                endNode.toElement().attribute("y").toDouble());
        QString endSign = startNode.toElement().attribute("signature").toUtf8();


//        qDebug() << "item is : " <<  ui->graphicsView->scene()->itemAt(startPos.rx() + 3, startPos.ry() + 3, QTransform());
        //checking if there is a pin inside scene with given position
        QGraphicsItem* itemstart = nullptr;
        if(itemstart = ui->graphicsView->scene()->itemAt(startPos.rx() + 3, startPos.ry() + 3, QTransform())){
            if(dynamic_cast<Pin*>(itemstart)){
               Pin* pin = dynamic_cast<Pin*>(itemstart);

               ui->graphicsView->conncontroller->receiving_pin(pin);
           }
        }

        QGraphicsItem* itemend = nullptr;
        if(itemend = ui->graphicsView->scene()->itemAt(endPos.rx() + 3, endPos.ry() + 3, QTransform())){
           if(dynamic_cast<Pin*>(itemend)){
               Pin* pin = dynamic_cast<Pin*>(itemend);

               ui->graphicsView->conncontroller->receiving_pin(pin);
           }
        }
    }


    file.close();
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
        update_component_menu();
    }
}

void MainWindow::on_actionpassive_triggered()
{
//    update_component_menu();

//    but->update_actions(lib);

}

void MainWindow::toolbar_button_action_receiver(QAction *act)
{
    if(lib->lib_has(act->text())){
        emit current_component(lib->lib_has(act->text()));
    }

    if(ui->toolBar->actions().size() == 3){

        ui->toolBar->removeAction(ui->toolBar->actions().at(2));
    }
    ui->toolBar->insertAction(ui->toolBar->actions().first(), act);

}

void MainWindow::update_component_menu()
{
    if(lib->headers().keys().isEmpty())
    return;
    //clear menu,set some new
    ui->menucomponents->clear();

//  add headers as actions
    int previous = -1;
    for(auto i : lib->headers().keys()){
        if(i != previous && !lib->headers().keys().isEmpty()){
            //list of pairs with header(name, parent)
            QList<QPair<QString, QString> > valuelist = lib->headers().values(i);
            for(auto j : valuelist){
                QAction* headerAction = ui->menucomponents->insert(j.first, j.second);
                if(headerAction){
                    headerAction->setObjectName(j.first);

//                    if(Component* tmpcomponent = lib->lib_has(headerAction->text())){
//                        headerAction->setIcon(QIcon(tmpcomponent->icon_path()));
//                        emit current_component(tmpcomponent);
//                    }
                }
            }
        }
        previous = i;
    }

    for(auto i : lib->components("all")){
        QAction* elementAction = ui->menucomponents->insert(i, lib->lib_has(i)->property("head").toString());
        if(elementAction){
            elementAction->setIcon(QIcon(lib->lib_has(i)->icon_path()));

            connect(elementAction, &QAction::triggered, [=](){
                toolbar_button_action_receiver(elementAction);
            });
        }
    }
}

QString MainWindow::update_sp_file()
{
//    qDebug() << "current_file : " << current_file;
    QStringList lst = ui->graphicsView->get_spice();
    QFile fout;
//    qDebug() << "aaa ";

    if(current_file == ""){
        fout.setFileName("unnamed.sp");
    }else{
//        qDebug() << "bbb ";

        fout.setFileName(*current_file.split(".").begin() + ".sp");

//        qDebug() << "ccc ";
    }

    if (fout.open(QFile::WriteOnly)) {
       QTextStream s(&fout);
       s << fout.fileName() << '\n';
       for (auto i : lst){
           s << i << '\n';
       }
       s << ".end" << '\n';
    }else {
       return QString();
    }
    fout.close();

    return fout.fileName();
}

void MainWindow::on_actionzoomout_triggered()
{
    ui->spinBox->stepDown();
}

void MainWindow::on_actionzoomin_triggered()
{
    ui->spinBox->stepUp();
}

void MainWindow::on_actionpoint_triggered()
{
    if(ui->actiongrab->isChecked())
    ui->actiongrab->trigger();
}

void MainWindow::on_actiongrab_triggered()
{
    if(ui->actiongrab->isChecked()){
        ui->graphicsView->setDragMode(QGraphicsView::DragMode::ScrollHandDrag);
        ui->graphicsView->grabFlag = true;
    }else{
        ui->graphicsView->setDragMode(QGraphicsView::DragMode::NoDrag);
        ui->graphicsView->grabFlag = false;
    }
}

void MainWindow::on_actionRun_triggered()
{
    //str - generated .sp file name
    QString str = update_sp_file();

    QProcess *process = new QProcess(this);
    QString ngspice = "bin/ngspice.exe";
    process->start(ngspice, QStringList({str}));

//    qDebug() << ui->graphicsView->get_spice();
}

void MainWindow::on_actionopen_sp_file_triggered()
{
    //str - generated .sp file name
    QString str = update_sp_file();

    QFileInfo info1(str);

    QString path1 = QDir::toNativeSeparators(info1.absolutePath());

    const QString path = QString(path1 + "\\" + str.split("/").last());


#ifdef Q_OS_WIN
           // Syntax is: explorer /select, "C:\Folder1\Folder2\file_to_select"
    if (QFileInfo(path).exists()) {

        STARTUPINFO startupInfo;
                ::ZeroMemory(&startupInfo, sizeof(startupInfo));
                startupInfo.cb = sizeof(startupInfo);

        PROCESS_INFORMATION processInfo;
                ::ZeroMemory(&processInfo, sizeof(processInfo));

        QString cmd = QString("explorer.exe /select,\"%1\"").arg(path);
        qDebug() << "cmd" << cmd;
        LPWSTR lpCmd = new WCHAR[cmd.size() + 1];
        cmd.toWCharArray(lpCmd);
        lpCmd[cmd.size()] = 0;

        bool ret = ::CreateProcessW(NULL, lpCmd, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo);
        qDebug() << "ret" << ret;
        delete [] lpCmd;

        if (ret) {
            ::CloseHandle(processInfo.hProcess);
            ::CloseHandle(processInfo.hThread);
        }
    }
#endif

}

//void Utils::Misc::openFolderSelect(const QString& absolutePath)
//{
//    const QString path = Utils::Fs::fromNativePath(absolutePath);
//#ifdef Q_OS_WIN
//    if (QFileInfo(path).exists()) {
//        // Syntax is: explorer /select, "C:\Folder1\Folder2\file_to_select"
//        // Dir separators MUST be win-style slashes

//        // QProcess::startDetached() has an obscure bug. If the path has
//        // no spaces and a comma(and maybe other special characters) it doesn't
//        // get wrapped in quotes. So explorer.exe can't find the correct path and
//        // displays the default one. If we wrap the path in quotes and pass it to
//        // QProcess::startDetached() explorer.exe still shows the default path. In
//        // this case QProcess::startDetached() probably puts its own quotes around ours.

//        STARTUPINFO startupInfo;
//        ::ZeroMemory(&startupInfo, sizeof(startupInfo));
//        startupInfo.cb = sizeof(startupInfo);

//        PROCESS_INFORMATION processInfo;
//        ::ZeroMemory(&processInfo, sizeof(processInfo));

//        QString cmd = QString("explorer.exe /select,\"%1\"").arg(Utils::Fs::toNativePath(absolutePath));
//        LPWSTR lpCmd = new WCHAR[cmd.size() + 1];
//        cmd.toWCharArray(lpCmd);
//        lpCmd[cmd.size()] = 0;

//        bool ret = ::CreateProcessW(NULL, lpCmd, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo);
//        delete [] lpCmd;

//        if (ret) {
//            ::CloseHandle(processInfo.hProcess);
//            ::CloseHandle(processInfo.hThread);
//        }
//    }
//    else {
//        // If the item to select doesn't exist, try to open its parent
//        openPath(path.left(path.lastIndexOf("/")));
//    }
//#elif defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
//    if (QFileInfo(path).exists()) {
//        QProcess proc;
//        QString output;
//        proc.start("xdg-mime", QStringList() << "query" << "default" << "inode/directory");
//        proc.waitForFinished();
//        output = proc.readLine().simplified();
//        if (output == "dolphin.desktop" || output == "org.kde.dolphin.desktop")
//            proc.startDetached("dolphin", QStringList() << "--select" << Utils::Fs::toNativePath(path));
//        else if (output == "nautilus.desktop" || output == "org.gnome.Nautilus.desktop"
//                 || output == "nautilus-folder-handler.desktop")
//            proc.startDetached("nautilus", QStringList() << "--no-desktop" << Utils::Fs::toNativePath(path));
//        else if (output == "caja-folder-handler.desktop")
//            proc.startDetached("caja", QStringList() << "--no-desktop" << Utils::Fs::toNativePath(path));
//        else if (output == "nemo.desktop")
//            proc.startDetached("nemo", QStringList() << "--no-desktop" << Utils::Fs::toNativePath(path));
//        else if (output == "kfmclient_dir.desktop")
//            proc.startDetached("konqueror", QStringList() << "--select" << Utils::Fs::toNativePath(path));
//        else
//            openPath(path.left(path.lastIndexOf("/")));
//    }
//    else {
//        // If the item to select doesn't exist, try to open its parent
//        openPath(path.left(path.lastIndexOf("/")));
//    }
//#else
//    openPath(path.left(path.lastIndexOf("/")));
//#endif
//}

void MainWindow::on_actionaction_undo_triggered()
{
    ui->graphicsView->actcontroller->undo();
}

void MainWindow::on_actionaction_redo_triggered()
{
    ui->graphicsView->actcontroller->redo();
}

void MainWindow::on_actiona1k_triggered()
{
    ui->graphicsView->actcontroller->undo();
}

void MainWindow::on_actionredo_triggered()
{
    ui->graphicsView->actcontroller->redo();
}


void MainWindow::on_actionOptions_triggered()
{

}

void MainWindow::on_actionPreferences_triggered()
{
     QDialog* set_prefs_dialog = new QDialog(this);
     set_prefs_dialog->setWindowTitle("Preferences");


     QVBoxLayout* pref_layout = new QVBoxLayout;

     QPushButton* button = new QPushButton;
     QLabel* label = new QLabel("Set background color : ");
     QHBoxLayout* vlay = new QHBoxLayout;

     vlay->addWidget(label);
     vlay->addWidget(button);

     connect(button, &QPushButton::clicked, [this](){
         QColorDialog* m_dialog = new QColorDialog(this);
         QColor background_color = m_dialog->getColor();
         ui->graphicsView->setBackgroundBrush(background_color);

     });

     QRadioButton* radbut1 = new QRadioButton;
     radbut1->setText("Hide port signatures");

     QRadioButton* radbut2 = new QRadioButton;
     radbut2->setText("Hide component names");

     pref_layout->addLayout(vlay);
     pref_layout->addWidget(radbut1);
     pref_layout->addWidget(radbut2);

     set_prefs_dialog->setLayout(pref_layout);

     switch(set_prefs_dialog->exec()){
     case 0:{

     break;
     }
     case 1:{

         if(radbut1->isChecked()){
             if(!ui->graphicsView->conncontroller->ports.isEmpty()){
                for(auto i : ui->graphicsView->conncontroller->ports){
//                    i->s
                }
             }
         }
     break;
     }
     default:
         break;
     }
}



void MainWindow::on_actionFind_triggered()
{

}
