#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setWindowTitle("C++分词工具");
    w.setWindowIcon(QIcon(":/img/img/icon.png"));
    w.setMinimumSize(1600, 800);
    return a.exec();
}
