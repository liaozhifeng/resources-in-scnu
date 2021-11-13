#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("XLEX生成器");
    w.setWindowIcon(QIcon(":/img/img/icon.png"));
    w.show();

    return a.exec();
}
