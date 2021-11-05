#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include "division.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_divisionBtn_clicked();

    void on_getMyLagBtn_clicked();

private:
    Ui::MainWindow *ui;
    QString fileName;
    division d;
};

#endif // MAINWINDOW_H
