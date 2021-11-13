#ifndef UTILS_H
#define UTILS_H
#include <QString>
#include <QStack>
#include <QCoreApplication>
#include <QList>
#include <QProcess>
#include <QFile>
#include <QSet>
#include <QMap>
#include <QPair>
#include <QtAlgorithms>
#include <QTableWidget>
#include <QLabel>

#include "graph.h"
#include "QDebug"
class utils
{
public:
    utils();

    QString getPostOrderEx(QString regEx);

    void getNfa(QString postEx);
    void drawNfa(QTableWidget *table, QLabel *label);
    void getDfa();
    void drawDfa(QTableWidget *table, QLabel *label);
    void minimizeDfa();
    void drawminiDfa(QTableWidget *table, QLabel *label);
    QString getCode();

private:
    QChar epsilon = QChar(0x03B5);
    QString dotPath;
    graph nfaGraph;
    graph dfaGraph;
    graph miniDfaGraph;

    QStack<int> stack;
    QSet<QChar> charSet;    //存储所有存在的字符
    int nfaStartNode;
    int nfaEndNode;

    int getPriority(QChar c);
    QString changeSymbol(QString regEx);    //[]转换成{|}
    QString addSymbol(QString regEx);       //添加并置符号&
    void basic(QChar c);    //基本正则表达式
    void apposition();      //并置 &
    void selection();       //选择 |
    void closure();         //闭包
    void optional();        //可选
    void positiveClosure(); //正闭包
    void epsilonClosure(int start, QSet<int> &set);
    QString setToQStringAndJudgeWhetherEnd(QSet<int> &set, bool &isEnd);
    void showTable(QTableWidget *table, graph graph);
};

#endif // UTILS_H
