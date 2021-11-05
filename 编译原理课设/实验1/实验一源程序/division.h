#ifndef DIVISION_H
#define DIVISION_H

#include <QString>
#include <QFile>
#include <QDebug>
#include <QTextEdit>
#include <QMap>

class division
{
public:
    division();
    void getDivision(QString fileName, QTextEdit * res);
    void getMyLanguage(QTextEdit * res);

private:
    const QString keywords[65] =
        {"asm", "else", "new", "this",
        "auto", "enum", "operator", "throw",
        "bool", "explicit", "private", "true",
        "break", "export", "protected", "try",
        "case", "extern", "public", "typedef",
        "catch", "false", "register", "typeid",
        "char", "float", "reinterpret_cast", "typename",
        "class", "for", "return", "union",
        "const", "friend", "short", "unsigned",
        "goto", "define", "cin", "cout",
        "const_cast", "signed", "using",
        "continue", "sizeof", "virtual",
        "default", "inline", "static", "void",
        "delete", "int", "static_cast", "volatile",
        "do", "long", "struct", "wchar_t",
        "double", "mutable", "switch", "while",
        "dynamic_cast", "namespace", "template"};

    const char symbols[24] = {'#', '(', ')', '{', '}', '+',
        '=', '<', '>', '!', '-', '/', '*', '?', ';', '&', '|', '.', ':', ',', '[', ']' ,'\'', '\''};

    const QString output[6] {
        "关键字", "特殊符号", "标识符", "数", "串", "注释"
    };

    const QString doubleSyms[11] = {"<<" , ">>", "&&", "||", "++", "--", "+=", "-=", "*=", "/=", "->"};

    const QString tab = "    ";
    bool isString = false;
    bool isComment = false;
    bool isNum = false;
    QString word = "";
    QString res = "";
    QString myLag = "";

    int judge(QString word);
    void readSentence(QString sentence);
    int removeBlank(QString sentence, int begin, int len);
    bool isKeywords(QString word);
    bool isDigit(QString word);
    bool isDoubleSyms(QString s);
    bool isSymbols(QCharRef c);
    bool _isString(QString s);
};

#endif // DIVISION_H
