#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSvgGenerator>
#include "Toolbar_elements/m_button.h"
#include "Components/library.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    m_button* but = new m_button;
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString input = ""; //remove from here

    QString current_file = "";
    Library* lib = nullptr;

public slots:
void slot1(QString text );

private slots:
void on_actionSave_triggered();
void on_actionOpen_triggered();
void on_actionadd_port_triggered();
void on_actionplain_connection_triggered();
void on_actioncurved_connection_triggered();
};

#endif // MAINWINDOW_H
