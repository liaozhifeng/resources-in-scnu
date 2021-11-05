#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    QString fileName = QFileDialog::getOpenFileName(
                    this, tr("open source file"),
                    "./", tr("Source files(*.cpp *.c *.txt);;All files (*.*)"));
    this->fileName = fileName;
    QFile file(fileName);
    QString res_ = "";
    if (file.open(QIODevice::ReadOnly)) {

        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            QString str(line);
            res_.append(line);
        }
    }
    file.close();
    ui->sourceText->setText(res_);
    ui->resText->setText("");
    ui->mylag->setText("");
    ui->sourceText->setReadOnly(true);
    qDebug() << fileName;
}



void MainWindow::on_divisionBtn_clicked()
{
    if (!fileName.isEmpty()) {
        d.getDivision(fileName, ui->resText);
        ui->resText->setReadOnly(true);
    } else {
        QMessageBox::warning(nullptr, "提示", "还没有选择文件");
    }

}

void MainWindow::on_getMyLagBtn_clicked()
{
    d.getMyLanguage(ui->mylag);
    ui->mylag->setReadOnly(true);
}
