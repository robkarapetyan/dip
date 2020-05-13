#include <QCheckBox>
#include <QComboBox>
#include <QFileDialog>
#include <QGraphicsItem>
#include "libeditor.h"
#include "ui_libeditor.h"
#include "Toolbar_elements/m_button.h"

LibEditor::LibEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LibEditor)
{
    ui->setupUi(this);

//    ui->right_tableWidgett-
//    _model = new PropertyTableModel(this);
//    ui->tableView->setModel(_model);


    connect(ui->treeWidget, SIGNAL(itemEntered(QTreeWidgetItem *, int )),
            this, SLOT(hovered_item_receiver(QTreeWidgetItem *)));
    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int )),
            this, SLOT(clicked_item_receiver(QTreeWidgetItem *)));

    QMenu* addmenu = new QMenu(ui->addButton);
    QAction* addHeader = addmenu->addAction("Header");
    QAction* addElement = addmenu->addAction("Element");

    connect(addHeader, SIGNAL( triggered(bool )), this,
            SLOT( addButton_addheader_clicked()));
    connect(addElement, SIGNAL( triggered(bool )), this,
            SLOT( addButton_addelement_clicked()));
    ui->addButton->setMenu(addmenu);
}

LibEditor::~LibEditor()
{
    delete ui;
}

void LibEditor::setLib(Library *lib_to_work_with)
{
    if(!lib_to_work_with)
        return;

    _lib = new Library;

    if (QFile::exists("libtmp.json"))
    {
        QFile::remove("libtmp.json");
    }

    QFile::copy("lib.json", "libtmp.json");
    _lib->set_lib_path("libtmp.json");

//    QFile saveFile("libtmp.json");

//    if (!saveFile.open(QIODevice::ReadOnly)) {
//        qWarning("Couldn't open lib file.");
//        return;
//    }

//    saveFile.close();
//    _lib->init();
}

void LibEditor::update_treewidget()
{
    if(!_lib)
    return;

    ui->treeWidget->clear();
    QTreeWidgetItem* rootitem = new QTreeWidgetItem;
    rootitem->setText(0, "components");
    rootitem->setWhatsThis(0, "locked");

    ui->treeWidget->addTopLevelItem(rootitem);

    int previous = -1;
    for(auto i : _lib->headers().keys()){
        if(i != previous){
            //list of pairs with header(name, parent)
            QList<QPair<QString, QString> > valuelist = _lib->headers().values(i);
            for(auto j : valuelist){
                QTreeWidgetItem* headeritem = new QTreeWidgetItem;
                headeritem->setText(0, j.first);
                headeritem->setWhatsThis(0, "editable_header");
                if(i == 0){
                    ui->treeWidget->setColumnCount(1);
                    rootitem->addChild(headeritem);
                    qDebug() << "adding to components" << j.first;
                }
                else{
                    QList<QTreeWidgetItem*> clist = ui->treeWidget->findItems(j.second, Qt::MatchContains|Qt::MatchRecursive, 0);
                    if(!clist.empty()){
                        clist.at(0)->addChild(headeritem);
                    }
                }
            }
        }

        previous = i;
    }

    QStringList strlist = _lib->components("all");
    for(auto i : strlist){
        Component* cmp = _lib->lib_has(i);
        if(cmp){
            QTreeWidgetItem* item1 = new QTreeWidgetItem;
            item1->setText(0, i);
            item1->setWhatsThis(0, "editable_component");

            QString head = cmp->property("head").toString();
            QList<QTreeWidgetItem*> clist = ui->treeWidget->findItems(head, Qt::MatchContains|Qt::MatchRecursive, 0);
            if(!clist.empty()){
                clist.at(0)->addChild(item1);
            }
        }
        delete cmp;
    }
}

//updating right side of window with mouse click received
void LibEditor::clicked_item_receiver(QTreeWidgetItem *item)
{
    QString item_name = item->text(0);
    if(_lib->lib_has(item_name)){
        ui->activeComponentLabel->setText(item_name);
        update_tableWidget(item_name);
    }
}

//updating preview label with mouse hover received
void LibEditor::hovered_item_receiver(QTreeWidgetItem *item)
{
    QString item_name = item->text(0);
    if(_lib->lib_has(item_name)){
        ui->label->setPixmap(QPixmap(_lib->lib_has(item_name)->icon_path()));
    }else{
        ui->label->setText("Preview");
    }
}

void LibEditor::update_tableWidget(const QString &component_name)
{
    //if library has component unparse properties to table model
    Component* cmp = _lib->lib_has(component_name);
    ui->right_tableWidgett->setColumnCount(3);
    ui->right_tableWidgett->setRowCount(cmp->dynamicPropertyNames().size());

    for (int j = 0; j < ui->right_tableWidgett->rowCount(); ++j){
        QTableWidgetItem* nameitem = new QTableWidgetItem();
        nameitem->setData(Qt::ItemDataRole::DisplayRole, cmp->dynamicPropertyNames().at(j));
        ui->right_tableWidgett->setItem(j, 0, nameitem);

        QTableWidgetItem* valueitem = new QTableWidgetItem();
        valueitem->setData(Qt::ItemDataRole::DisplayRole,
                           cmp->property(cmp->dynamicPropertyNames().at(j)));
        ui->right_tableWidgett->setItem(j, 1, valueitem);
         //also state :: e.g  mOhm Ohm MOhm
        if(cmp->tri_states_map.contains(cmp->dynamicPropertyNames().at(j) + "_tri_state")){
            QComboBox* comboitem = new QComboBox(this);
            comboitem->addItem(cmp->tri_states_map[cmp->dynamicPropertyNames().at(j) + "_tri_state_active"]);
            ui->right_tableWidgett->setCellWidget(j, 2, comboitem);
        }
    }
}

// find action
//completed
void LibEditor::on_findButton_clicked()
{
    //open new dialog with lineedit( input) and < found>'s list
    QDialog* find_dialog = new QDialog(this, Qt::WindowFlags(0));
    QVBoxLayout* dialog_layout = new QVBoxLayout();
    m_button* dialog_main_button = new m_button();

    //main button setup
    dialog_main_button->add_action(QPixmap("C:/Users/Rob/Documents/diplom_beginning/icons/resh.ico"),"Resistor");
    dialog_main_button->add_action(QPixmap("C:/Users/Rob/Documents/diplom_beginning/icons/caph.png"),"Capacitor");
    dialog_main_button->add_action("Inductor");

    //buttonbox setup
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                        | QDialogButtonBox::Cancel, this);

    connect(buttonBox, SIGNAL(accepted()), find_dialog, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), find_dialog, SLOT(reject()));

    connect(dialog_main_button, SIGNAL( action_triggered(QAction*)), this,
               SLOT( find_dialog_action_received(QAction *)));
    connect(dialog_main_button, SIGNAL( action_triggered(QAction*)), find_dialog,
               SLOT( accept()));

    //layout setup
    dialog_layout->addWidget(dialog_main_button->Actions());
    dialog_layout->addWidget(buttonBox);
    find_dialog->setLayout(dialog_layout);

    switch(find_dialog->exec()){
    case 0:{
        break;
    }
    case 1:{
        if(!find_dialog_action){
            find_dialog_action = new QAction(dialog_main_button->lineedit_current_text());
            QList<QTreeWidgetItem*> clist = ui->treeWidget->findItems(find_dialog_action->text(), Qt::MatchContains|Qt::MatchRecursive, 0);
            ui->treeWidget->setCurrentItem(clist.at(0));
            delete find_dialog_action;
        }else{
            QList<QTreeWidgetItem*> clist = ui->treeWidget->findItems(find_dialog_action->text(), Qt::MatchContains|Qt::MatchRecursive, 0);
            if(!clist.isEmpty()){
                ui->treeWidget->setCurrentItem(clist.at(0));
            }
            find_dialog_action = nullptr;
        }
        break;
    }
    default:
        break;
    }
}

void LibEditor::find_dialog_action_received(QAction * act)
{
    if(act)
        find_dialog_action = act;
}

// edit action
void LibEditor::on_editButton_clicked()
{
    if(!ui->treeWidget->currentItem() || !_lib || !ui->treeWidget->currentItem()->parent())
        return;

    if(ui->treeWidget->currentItem()->whatsThis(0) == "locked"){
        QMessageBox* cantEdit = new QMessageBox(this);
        cantEdit->setText("Modify of current item is disabled.");
        cantEdit->setWindowTitle("Modify error");
        cantEdit->setIcon(QMessageBox::Warning);
        cantEdit->exec();
        return;
    }

    if(ui->treeWidget->currentItem()->whatsThis(0) == "editable_header"){
        bool ok;
        QString i = QInputDialog::getText(this, tr("Rename Header"),
                                                      tr("Header name"), QLineEdit::EchoMode::Normal,
                                          ui->treeWidget->currentItem()->text(0), &ok);
        if (ok)
        {
           if(i != ""){
               _lib->rename_header(ui->treeWidget->currentItem()->text(0), i);

               ui->treeWidget->currentItem()->setText(0, i);
               //update lib's headers
           }
        }
        return;
    }

    //replace this -----------------------------------------------------------------------------
    if(ui->treeWidget->currentItem()->whatsThis(0) == "editable_component"){
        bool ok;
        QString i = QInputDialog::getText(this, tr("Edit component"),
                                                      tr("component name"), QLineEdit::EchoMode::Normal,
                                          ui->treeWidget->currentItem()->text(0), &ok);
        if (ok)
        {
           if(i != ""){
               ui->treeWidget->currentItem()->setText(0, i);
           }
        }
        return;
    }
//----------------------------------------------------------------------------

}

void LibEditor::editButton_text_receiver()
{
    ui->treeWidget->currentItem()->setText(0, editButton_lineedit->text());
//   qDebug() <<  ui->editButton->menu()->defaultAction()->text();
//    qDebug() << ui->treeWidget->currentItem()->whatsThis(0);
    //command queue <<  rename (oldHeader_name) (newHeader_name) | replace (Component*) (Component*)
}

//remove action
//completed removal from lib done
void LibEditor::on_removeButton_clicked()
{
    if(!ui->treeWidget->currentItem() || !_lib || !ui->treeWidget->currentItem()->parent())
        return;

    if(ui->treeWidget->currentItem()->whatsThis(0) == "locked"){
        QMessageBox* cantDelete = new QMessageBox(this);
        cantDelete->setText("Deletion of current item is disabled.");
        cantDelete->setWindowTitle("Deletion error");
        cantDelete->setIcon(QMessageBox::Warning);
        cantDelete->exec();
        return;
    }

    if(!remove_messagebox){
        remove_messagebox = new QMessageBox(this);

        remove_messagebox->setIcon(QMessageBox::Warning);
        remove_messagebox->setText("If you delete the component, you can't \nget back it anymore.Unless you create it again.");
        remove_messagebox->setWindowTitle("Warning");
        remove_messagebox->setCheckBox(new QCheckBox("Don't show again."));
        remove_messagebox->addButton("Cancel", QMessageBox::RejectRole);
        remove_messagebox->addButton("Ok", QMessageBox::AcceptRole);
    }

    if(remove_messagebox->checkBox()->isChecked()){
        QString current = ui->treeWidget->currentItem()->text(0);
        if(ui->treeWidget->currentItem()->whatsThis(0) == "editable_component"){
            _lib->remove_component(current);
        }
        ui->treeWidget->currentItem()->parent()->removeChild(ui->treeWidget->currentItem());

        //command queue << remove whatisthis
        //undo redo add whatisthis
        return;
    }

    switch(remove_messagebox->exec()){
    case 0:{
        remove_messagebox->close();
        break;
    }
    case 1:{
        QString current = ui->treeWidget->currentItem()->text(0);
        if(ui->treeWidget->currentItem()->whatsThis(0) == "editable_component"){

            _lib->remove_component(current);
        }
        ui->treeWidget->currentItem()->parent()->removeChild(ui->treeWidget->currentItem());

//        ui->treeWidget->removeItemWidget(ui->treeWidget->currentItem(), 0);
        //command queue << remove whatisthis
        //undo redo add whatisthis
        break;
    }
    default:
        break;
    }
}

// add action
void LibEditor::addButton_addheader_clicked()
{
    if(!_lib)
        return;

    bool ok;

    QString i = QInputDialog::getText(this, tr("Add Header"),
                                                  tr("Header name"), QLineEdit::EchoMode::Normal, QString("smtg"), &ok);
    if (ok)
    {
       if(i != ""){
           QTreeWidgetItem* user_rootitem = new QTreeWidgetItem;
           user_rootitem->setWhatsThis(0, "editable_header");
           user_rootitem->setText(0, i);
           //?
//           user_rootitem->setFlags(Qt::ItemFlag::ItemIsEditable | Qt::ItemFlag::ItemIsEnabled);
           //?

           QList<QTreeWidgetItem*> clist = ui->treeWidget->findItems(i, Qt::MatchFlag::MatchExactly|Qt::MatchRecursive, 0);
            if(!clist.isEmpty()){
                QMessageBox* duplicateWarn = new QMessageBox(this);
                duplicateWarn->deleteLater();
                duplicateWarn->setIcon(QMessageBox::Warning);
                duplicateWarn->setText("There is already a header named '" + i + "'");
                duplicateWarn->setWindowTitle("Warning");
                duplicateWarn->setCheckBox(new QCheckBox("Don't show again."));
                duplicateWarn->addButton("Ok", QMessageBox::AcceptRole);
                duplicateWarn->checkBox()->setChecked(false);
                duplicateWarn->exec();
                return;
            }
            qDebug() << "adding " << i << ", to " << ui->treeWidget->currentItem()->text(0);
            if(ui->treeWidget->currentItem()->parent()){
                _lib->add_header(i, ui->treeWidget->currentItem()->text(0));
            }else{
                _lib->add_header(i, "");
            }

           if(ui->treeWidget->currentItem()){
               ui->treeWidget->currentItem()->addChild(user_rootitem);
           }else{
               ui->treeWidget->topLevelItem(0)->addChild(user_rootitem);
           }
           ui->treeWidget->scrollToItem(user_rootitem);

       }
    }
    //command queue << add str(header name)
    //undo redo << remove str(header name)
}

void LibEditor::addButton_addelement_clicked()
{
    if(!ui->treeWidget->currentItem() || !_lib || !ui->treeWidget->currentItem()->parent())
        return;

    //open qdialog with empty component properties unparsed
    QDialog* add_element_dialog = new QDialog(this);
    QGridLayout* add_dialog_layout = new QGridLayout(add_element_dialog);
    add_element_dialog->deleteLater();
    add_element_dialog->setWindowTitle("Add Element to " + ui->treeWidget->currentItem()->text(0));
    Component* component_instance = new Component;

    QTableWidget* tablewidget1 = new QTableWidget(this);
    tablewidget1->setColumnCount(3);
    tablewidget1->setHorizontalHeaderLabels({"name", "value", "state"});

    QGraphicsSimpleTextItem* mode = new QGraphicsSimpleTextItem;
    mode->setText("Edit mode");
    mode->setPos(0,0);
//    mode->setFlag(ItemIsMovable, false);
    QGraphicsScene* scene = new QGraphicsScene(add_element_dialog);
    QGraphicsView* view = new QGraphicsView(add_element_dialog);

    view->setScene(scene);
    view->setFixedSize(QSize(100, 100));

    scene->setSceneRect(QRectF(0,0, 90,90));
    scene->addItem(component_instance);
    scene->addItem(mode);
    component_instance->moveBy(40,40);
    component_instance->setFlag(QGraphicsObject::ItemIsMovable, false);

    QPushButton* setObjNameButton = new QPushButton(add_element_dialog);
    setObjNameButton->setText("Set Component Name");
    QPushButton* setIconButton = new QPushButton(add_element_dialog);
    setIconButton->setText("Set Icon");
    QPushButton* addPinButton = new QPushButton(add_element_dialog);
    addPinButton->setText("Add Pin");
    QPushButton* removePinButton = new QPushButton(add_element_dialog);
    removePinButton->setText("Remove Pin");
    removePinButton->setCheckable(true);
    QPushButton* addPropertyButton = new QPushButton(add_element_dialog);
    addPropertyButton->setText("Add Property");
    QPushButton* addMultiStatePropertyButton = new QPushButton(add_element_dialog);
    addMultiStatePropertyButton->setText("Add multistate Property");
    QPushButton* removePropertyButton = new QPushButton(add_element_dialog);
    removePropertyButton->setText("Remove selected Property");

    connect(setIconButton, &QPushButton::clicked, [component_instance, this](){
        QString newPath = QFileDialog::getOpenFileName(this, "Open icon file",
                                                            QString(), tr("files (*.ico *.png)"));
        if (newPath.isEmpty())
            return;
        component_instance->set_pixmap(newPath);
    });

    connect(addPinButton, &QPushButton::clicked, [component_instance, removePinButton, mode](){
        if(removePinButton->isChecked()){
            removePinButton->setChecked(false);
            mode->setText("Edit mode");
            component_instance->blockSignals(true);
        }
        component_instance->add_pin(0,0);
        component_instance->make_pins_movable();
    });

    connect(setObjNameButton, &QPushButton::clicked, [component_instance, removePinButton, mode, this](){
        if(removePinButton->isChecked()){
            removePinButton->setChecked(false);
            mode->setText("Edit mode");
            component_instance->blockSignals(true);
        }
        bool ok;

        QString i = QInputDialog::getText(this, tr("Set component name"),
                                                      tr("Component name : "), QLineEdit::EchoMode::Normal, QString(""), &ok);
        if (ok && i != "")
        {
            component_instance->setObjectName(i);
        }
    });

    connect(removePinButton, &QPushButton::clicked, [mode, component_instance, scene, removePinButton, this](){
        if(removePinButton->isChecked()){
            component_instance->blockSignals(false);
            mode->setText("Pin remove mode");
            connect(component_instance, &Component::pin_hover_signal,
                    [scene] (Pin* pin){
                scene->removeItem(pin);
            });
        }else{
            mode->setText("Edit mode");
            component_instance->blockSignals(true);
        }

    });

    connect(addPropertyButton, &QPushButton::clicked, [component_instance, removePinButton, mode, tablewidget1, this](){
        if(removePinButton->isChecked()){
            removePinButton->setChecked(false);
            mode->setText("Edit mode");
            component_instance->blockSignals(true);
        }
        QDialog* add_prop_dialog = new QDialog(this);
//        add_prop_dialog->deleteLater();
        QGridLayout* layout1 = new QGridLayout;
        layout1->addWidget(new QLabel("Property name : ", add_prop_dialog),0,0);

        QLineEdit* prop_name = new QLineEdit(add_prop_dialog);
        prop_name->setPlaceholderText("name");
        layout1->addWidget(prop_name, 0,1);

        QDialogButtonBox* buttonbox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                           | QDialogButtonBox::Cancel);
        connect(buttonbox, &QDialogButtonBox::accepted, add_prop_dialog, &QDialog::accept);
        connect(buttonbox, &QDialogButtonBox::rejected, add_prop_dialog, &QDialog::reject);
        layout1->addWidget(buttonbox, 1,0, 1,3);

        add_prop_dialog->setLayout(layout1);
        switch(add_prop_dialog->exec()){
        case 0:{

        break;
        }
        case 1:{
            if(prop_name->text() != ""){
                component_instance->setProperty(prop_name->text().toUtf8(), 0);
                tablewidget1->setRowCount(tablewidget1->rowCount() + 1);

                QTableWidgetItem* nameitem1 = new QTableWidgetItem();
                nameitem1->setData(Qt::ItemDataRole::DisplayRole, prop_name->text());
                tablewidget1->setItem(tablewidget1->rowCount() - 1, 0, nameitem1);

                QTableWidgetItem* valueitem1 = new QTableWidgetItem();
                valueitem1->setData(Qt::ItemDataRole::DisplayRole, 0);
                tablewidget1->setItem(tablewidget1->rowCount() - 1, 1, valueitem1);
            }
        break;
        }
        default:
            break;
        }


    });

    connect(addMultiStatePropertyButton, &QPushButton::clicked, [component_instance, removePinButton, mode, tablewidget1, this](){
        if(removePinButton->isChecked()){
            removePinButton->setChecked(false);
            mode->setText("Edit mode");
            component_instance->blockSignals(true);
        }
        QDialog* add_tristateprop_dialog = new QDialog(this);
        add_tristateprop_dialog->deleteLater();
        QGridLayout* layout1 = new QGridLayout;
        layout1->addWidget(new QLabel("Property name : ", add_tristateprop_dialog),0,0);
        layout1->addWidget(new QLabel("Property states : ", add_tristateprop_dialog), 1,0);
        QLineEdit* prop_name = new QLineEdit( add_tristateprop_dialog);
        QLineEdit* prop_states = new QLineEdit( add_tristateprop_dialog);
        prop_name->setPlaceholderText("name");
        prop_states->setPlaceholderText("seperated with ','");

        layout1->addWidget(prop_name, 0,2);
        layout1->addWidget(prop_states, 1,2);

        QDialogButtonBox* buttonbox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                           | QDialogButtonBox::Cancel);
        connect(buttonbox, &QDialogButtonBox::accepted, add_tristateprop_dialog, &QDialog::accept);
        connect(buttonbox, &QDialogButtonBox::rejected, add_tristateprop_dialog, &QDialog::reject);
//        add_tristateprop_dialog->

        layout1->addWidget(buttonbox, 2,0, 2,3);
        add_tristateprop_dialog->setLayout(layout1);
        switch(add_tristateprop_dialog->exec()){
        case 0:{

        break;
        }
        case 1:{
            if(prop_name->text() != "" && prop_states->text() != ""){
                component_instance->setProperty(prop_name->text().toUtf8(), 0);
                component_instance->add_tri_state_prop(prop_name->text(), prop_states->text());

                tablewidget1->setRowCount(tablewidget1->rowCount() + 1);

                QTableWidgetItem* nameitem1 = new QTableWidgetItem();
                nameitem1->setData(Qt::ItemDataRole::DisplayRole, prop_name->text());
                tablewidget1->setItem(tablewidget1->rowCount() - 1, 0, nameitem1);

                QTableWidgetItem* valueitem1 = new QTableWidgetItem();
                valueitem1->setData(Qt::ItemDataRole::DisplayRole, 0);
                tablewidget1->setItem(tablewidget1->rowCount() - 1, 1, valueitem1);

                QComboBox* comboitem = new QComboBox(this);
                comboitem->addItem(prop_states->text().split(",").at(0));
                tablewidget1->setCellWidget(tablewidget1->rowCount() - 1, 2, comboitem);
            }
        break;
        }
        default:
            break;
        }


    });

    QMessageBox* remove_messagebox1 = new QMessageBox(this);

    remove_messagebox1->setIcon(QMessageBox::Warning);
    remove_messagebox1->setText("If you delete the component, you can't \nget back it anymore.Unless you create it again.");
    remove_messagebox1->setWindowTitle("Warning");
    remove_messagebox1->setCheckBox(new QCheckBox("Don't show again."));
    remove_messagebox1->addButton("Cancel", QMessageBox::RejectRole);
    remove_messagebox1->addButton("Ok", QMessageBox::AcceptRole);
    remove_messagebox1->checkBox()->setChecked(false);

    connect(removePropertyButton, &QPushButton::clicked, [remove_messagebox1, component_instance, tablewidget1, this](){

        if(!tablewidget1->currentItem()){
            return;
        }
    //        remove_messagebox->deleteLater();

        if(remove_messagebox1->checkBox()->isChecked()){
            component_instance->setProperty(tablewidget1->item(tablewidget1->currentRow(), 0)->text().toUtf8(), QVariant());
            component_instance->remove_tri_state_prop(tablewidget1->item(tablewidget1->currentRow(), 0)->text());
            tablewidget1->removeRow(tablewidget1->currentRow());
            return;
        }

        switch(remove_messagebox1->exec()){
        case 0:{
            remove_messagebox1->close();
            break;
        }
        case 1:{
            component_instance->setProperty(tablewidget1->item(tablewidget1->currentRow(), 0)->text().toUtf8(), QVariant());
            component_instance->remove_tri_state_prop(tablewidget1->item(tablewidget1->currentRow(), 0)->text());
            tablewidget1->removeRow(tablewidget1->currentRow());

            break;
        }
        default:
            break;
        }
    });

    QDialogButtonBox* buttonbox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                       | QDialogButtonBox::Cancel);
    connect(buttonbox, &QDialogButtonBox::accepted, [component_instance, add_element_dialog, this]{
        if(component_instance->objectName() == "" || component_instance->icon_path() == "" ||
                component_instance->dynamicPropertyNames().empty()){
            QMessageBox* b = new QMessageBox(this);
            b->setIcon(QMessageBox::Warning);
            b->setText("Some values wasn't set properly!\n check!\tComponent name,\n\t icon,\n\t properties not empty.");
            b->setWindowTitle("Warning");
            b->setAttribute(Qt::WA_DeleteOnClose, true);
            QPushButton* okbutton = b->addButton("Ok", QMessageBox::AcceptRole);
            b->show();
            connect(okbutton, &QPushButton::clicked, [b]() {
                b->close();
            });
        }else{
            add_element_dialog->accept();
        }
    });
    connect(buttonbox, &QDialogButtonBox::rejected, add_element_dialog, &QDialog::reject);

    add_dialog_layout->addWidget(view, 0,0);
    add_dialog_layout->addWidget(setIconButton, 1,0);
    add_dialog_layout->addWidget(setObjNameButton, 2,0);
    add_dialog_layout->addWidget(addPinButton, 3,0);
    add_dialog_layout->addWidget(removePinButton, 4,0);
    add_dialog_layout->addWidget(addPropertyButton, 5,0);
    add_dialog_layout->addWidget(addMultiStatePropertyButton, 6,0);
    add_dialog_layout->addWidget(removePropertyButton, 7,0);
    add_dialog_layout->addWidget(tablewidget1, 0,1, 8,1);
    add_dialog_layout->addWidget(buttonbox, 8,0, 8,2);

    add_element_dialog->setLayout(add_dialog_layout);

    switch(add_element_dialog->exec()){
    case 0:{

    break;
    }
    case 1:{
        QTreeWidgetItem* user_elementitem = new QTreeWidgetItem;
        user_elementitem->setWhatsThis(0, "editable_component");
        user_elementitem->setText(0, component_instance->objectName());


        if(ui->treeWidget->currentItem()){
            ui->treeWidget->currentItem()->addChild(user_elementitem);
        }else{
            ui->treeWidget->topLevelItem(0)->addChild(user_elementitem);
        }
        ui->treeWidget->scrollToItem(user_elementitem);

        component_instance->setProperty("head", ui->treeWidget->currentItem()->text(0));
        _lib->add_component(component_instance);

    break;
    }
    default:
        break;
    }

    /*if(!ui->treeWidget->currentItem() || !_lib || !ui->treeWidget->currentItem()->parent())
//        return;

//    bool ok;

//    QString i = QInputDialog::getText(this, tr("Add Element"),
//                                                  tr("Element Name"), QLineEdit::EchoMode::Normal, QString("smtg"), &ok);
//    if (ok)
//    {
//       if(i != ""){
//           QTreeWidgetItem* user_elementitem = new QTreeWidgetItem;
//           user_elementitem->setWhatsThis(0, "editable_component");
//           user_elementitem->setText(0, i);
//           //?
////           user_rootitem->setFlags(Qt::ItemFlag::ItemIsEditable | Qt::ItemFlag::ItemIsEnabled);
//           //?
//           if(ui->treeWidget->currentItem()){
//               ui->treeWidget->currentItem()->addChild(user_elementitem);
//           }else{
//               ui->treeWidget->topLevelItem(0)->addChild(user_elementitem);
//           }
//           ui->treeWidget->scrollToItem(user_elementitem, QAbstractItemView::ScrollHint::PositionAtBottom);
//       }
//    }*/


    //command queue << add Component*
    //undo redo << remove Component*
}

// accept / reject
void LibEditor::on_buttonBox_accepted()
{
    if (QFile::exists("lib.json"))
    {
        QFile::remove("lib.json");
    }

    QFile::copy("libtmp.json", "lib.json");
    QFile::remove("libtmp.json");
}

void LibEditor::on_buttonBox_rejected()
{
    if (QFile::exists("libtmp.json"))
    {
        QFile::remove("libtmp.json");
    }
}

