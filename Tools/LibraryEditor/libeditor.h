#ifndef LIBEDITOR_H
#define LIBEDITOR_H

#include <QDialog>
#include <QTreeView>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "Components/library.h"

//#include "Tools/model.h"
//#include "Tools/propertytablemodel.h"


namespace Ui {
class LibEditor;
}

class LibEditor : public QDialog
{
    Q_OBJECT

public:
    explicit LibEditor(QWidget *parent = nullptr);
    ~LibEditor();

    void setLib(Library* lib_to_work_with);

    void update_treewidget();
    void update_tableWidget(const QString& component_name);

private:
    Ui::LibEditor *ui;
private:
    Library* _lib = nullptr;

    //IEditorCommand command queue
    //IEditorCommand undo stack
    //               redo stack

    QAction* find_dialog_action = nullptr;
    QMessageBox* remove_messagebox = nullptr;
    QLineEdit* editButton_lineedit = nullptr;

public slots:
    void clicked_item_receiver(QTreeWidgetItem* item);
    void hovered_item_receiver(QTreeWidgetItem * item);
signals:
    void lib_modifications_accepted(const Library&);

private slots:
    void on_findButton_clicked();
    void find_dialog_action_received(QAction*);
    void on_editButton_clicked();
    void editButton_text_receiver();
    void on_removeButton_clicked();
    void addButton_addheader_clicked();
    void addButton_addelement_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

#endif // LIBEDITOR_H
