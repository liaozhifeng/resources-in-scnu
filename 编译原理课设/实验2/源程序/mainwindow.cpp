#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
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

void MainWindow::on_analysisButton_clicked()
{
    QString regEx = ui->regexInput->toPlainText();
    utils u;

    auto table = ui->nfaTable;
    u.getNfa(regEx);
    u.drawNfa(table, ui->nfaImg);
    u.getDfa();
    u.drawDfa(ui->dfaTable, ui->dfaImg);
    u.minimizeDfa();
    u.drawminiDfa(ui->miniTable, ui->minidfaImg);
    QString code = u.getCode();
    ui->codeText->setText(code);
    ui->codeText->setReadOnly(true);
}

void MainWindow::on_saveButton_clicked()
{
    QString code = ui->codeText->toPlainText();
    QString filePath = QFileDialog::getSaveFileName(this, "save source file", ".", tr("source file(*.cpp)"));
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "保存文件出错", "some wrong may happen");
        return;
    }
    QTextStream stream(&file);
    stream << code;
    file.close();
    QMessageBox::information(this, "提示", "保存成功");
}
