#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSvgGenerator>
#include "Toolbar_elements/m_button.h"
#include "Components/library.h"
#include "Tools/LibraryEditor/libeditor.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    m_button* but = nullptr;
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString input = ""; //remove from here

    QString current_file = "";
    Library* lib = nullptr;
    LibEditor* editor = nullptr;

private slots:
    void on_actionSave_triggered();

    void on_actionOpen_triggered();

    void on_actionadd_port_triggered();

    void on_actionplain_connection_triggered();

    void on_actioncurved_connection_triggered();

    void on_actionLibrary_Editor_triggered();

//    void on_actionaction_grid_triggered();
    void on_actionpassive_triggered();

    void toolbar_button_action_receiver(QAction*);

    void on_actionzoomout_triggered();

    void on_actionzoomin_triggered();

    void on_actionpoint_triggered();

    void on_actiongrab_triggered();

    void on_actionRun_triggered();

    void on_actionopen_sp_file_triggered();

    void on_actionaction_undo_triggered();

    void on_actionaction_redo_triggered();

    void on_actionOptions_triggered();

    void on_actionPreferences_triggered();

    void on_actiona1k_triggered();

    void on_actionredo_triggered();

    void on_actionFind_triggered();

private:
//    QAction* find_action(QAction*, const QString&);
    void update_component_menu();

    QString update_sp_file();

//    QAction* menu_contains(QMenu* menu, const QString& header_name);
signals:
    void current_component(Component*);
};

#endif // MAINWINDOW_H
