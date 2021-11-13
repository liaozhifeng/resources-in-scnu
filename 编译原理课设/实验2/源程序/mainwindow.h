#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDebug>
#include <QList>
#include <QFileDialog>
#include <QMessageBox>

#include "utils.h"
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
    void on_analysisButton_clicked();

    void on_saveButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
