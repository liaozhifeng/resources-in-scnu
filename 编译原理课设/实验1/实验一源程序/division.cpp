#include "division.h"

division::division() {}

void division::getDivision(QString fileName, QTextEdit * resText)
{
    QFile file(fileName);
    res = "";
    myLag = "";

    if (file.open(QIODevice::ReadOnly)) {
        QTextStream fin(&file);
        QString line = fin.readLine();
        while (!line.isNull()) {
            readSentence(line);
            line = fin.readLine();
        }
    }
    resText->setText(res);
    file.close();
}
bool division::isKeywords(QString word)
{
    uint t;
    uint len = sizeof (keywords) / sizeof (keywords[0]);
    for (t = 0; t < len; t++) {
        if (word == keywords[t]) {
            break;
        }
    }
    return t != len;
}

bool division::isDigit(QString word)
{
    if (word[0] >= '0' && word[0] <= '9')
        return true;
    if (word[0] == '-' || word[0] == '+')
        return true;
    return false;
}

bool division::isDoubleSyms(QString s)
{
    uint len = sizeof (doubleSyms)/ sizeof (doubleSyms[0]);
    uint t;
    for(t=0;t<len;t++)
    {
        if(s == doubleSyms[t])
            break;
    }
    return t != len;
}

bool division::isSymbols(QCharRef c)
{
    uint t = 0;
    uint len = sizeof (symbols) / sizeof (symbols[0]);
    for(t = 0; t < len; t++) {
        if (c == symbols[t]) {
            break;
        }
    }
    return t != len;
}

bool division::_isString(QString s)
{
    if (s[0] == '"' || s[s.length() - 1] == '"')
        return true;
    return false;
}


int division::judge(QString word)
{
    if (isKeywords(word)) return 0;
    else if (isDigit(word)) return 3;
    else if (_isString(word)) return 4;
    else return 2;
}

int division::removeBlank(QString sentence, int begin, int len)
{
    while(sentence[begin] == ' ' && begin <= len - 1) {
        begin++;
        myLag.append(" ");
    }
    return begin;
}

void division::readSentence(QString sentence)
{
    int begin = 0;

    if(!isComment) {
        removeBlank(sentence, begin, sentence.length());
        sentence = sentence.simplified();
    }
    begin = 0;
    int len = sentence.length();
    if (isComment) {
        int i = 0;
        for(i = begin; i < len; i++) {
            if (i!=len - 1 && sentence[i] == '*' && sentence[i+1] == '/') {
                res.append("*/" + tab + output[5] + "\n");
                myLag.append("*/");
                isComment = false;
                break;
            } else {
                res.append(sentence[i]);
                myLag.append(sentence[i]);
            }
        }
        if (isComment) {
            res.append("\n");
            myLag.append("\n");
            return;
        }
        begin = i + 2;
    }

    if (!isString) {
        begin = removeBlank(sentence, begin, len);
    }

    while (begin <= len - 1) {
        if(!isString && begin != len - 1 && sentence[begin] == '/' && sentence[begin + 1] == '/') {
            res.append(sentence);
            res.append(tab + output[5] + "\n");
            myLag.append(sentence + "\n");
            return;
        }
        else if (!isString && begin != len - 1 && sentence[begin] == '/' && sentence[begin + 1] == '*') {
            isComment = true;
            int t = begin;
            for(t = begin; t<len;t++) {
                if (t!=len - 1 && sentence[t] == '*' && sentence[t+1] == '/') {
                    res.append("*/" + tab + output[5] + "\n");
                    myLag.append("*/");
                    isComment = false;
                    begin = t + 2;
                    break;
                } else {
                    res.append(sentence[t]);
                    myLag.append(sentence[t]);
                }
            }
            if (isComment) {
                res.append("\n");
                myLag.append("\n");
                return;
            }
            else {
                continue;
            }

        }
        else if (!isString && sentence[begin] == '#') {
            res.append("#" + tab + output[1] + "\n");
            myLag.append("#");
            begin++;

            //去除#后面的空格
            begin = removeBlank(sentence, begin, len);

            if (sentence[begin] == 'i') {
                res.append("include" + tab + output[0] + "\n");
                myLag.append("INCLUDE");
                begin += 7;

                //去除include后面的空格
                begin = removeBlank(sentence, begin, len);

                while(begin <= len - 1) {
                    if (sentence[begin] == ' ')
                        break;
                    res.append(sentence[begin]);
                    myLag.append(sentence[begin]);
                    begin++;
                }
                res.append(tab + output[1] + "\n");
                myLag.append("\n");
                return;
            } else {
                res.append("define" + tab + output[0] + "\n");
                myLag.append("DEFINE");
                begin += 6;

                begin = removeBlank(sentence, begin, len);

                while(begin <= len - 1) {
                    if (sentence[begin] == ' ')
                        break;
                    res.append(sentence[begin]);
                    myLag.append(sentence[begin]);
                    begin++;
                }
                res.append(tab + output[2] + "\n");
                myLag.append("\n");

                begin = removeBlank(sentence, begin, len);
                int res_ = 3;
                if (sentence[begin] == '"') res_ = 4;

                while (begin <= len - 1) {
                    if (sentence[begin] != ' ' || res_ ==4)
                           res.append(sentence[begin]);
                    myLag.append(sentence[begin]);
                    begin++;
                }
                myLag.append("\n");
                res.append(tab + output[res_] + "\n");
            }

        } else if (!isString&&isSymbols(sentence[begin])) {
            QCharRef c = sentence[begin];

            if(sentence[begin] == '.' && !word.isEmpty() && ((word[0]>='0'&&word[0]<='9')||word[0]=='-' || word[0]=='+')) {
                word+='.';
                begin++;
                continue;
            }  else if ((sentence[begin] == '-' || sentence[begin] == '+') && isNum) {
                if (begin < len - 1 && sentence[begin + 1] <= '9' && sentence[begin + 1] >= '0') {
                    word += sentence[begin];
                    begin++;
                    continue;
                }
            }
            isNum = true;
            if (!word.isEmpty()) {
                int res_ = judge(word);
                res.append(word + tab + output[res_] + "\n");
                if (res_ == 0) {
                    myLag.append(word.toUpper());
                } else {
                    myLag.append(word);
                }
                word = "";
            }

            QString temp = "";
            temp += c;
            if(begin != len - 1 && isSymbols(sentence[begin + 1])) {
                temp += sentence[begin + 1];
                if (begin < len - 2 && ((sentence[begin] == '<' && sentence[begin +1 ] == '<' && sentence[begin + 2] == '=')||
                    (sentence[begin] == '>' && sentence[begin +1] == '>' && sentence[begin + 2] == '='))) {
                    res.append("" + sentence[begin] + sentence[begin+1] + sentence[begin + 2] + tab + output[1] + "\n");
                    myLag.append("" + sentence[begin] + sentence[begin + 1] + sentence[begin + 2]);
                    begin+=2;
                } else if (isDoubleSyms(temp)) {
                    begin++;
                    res.append(temp + tab + output[1] + "\n");
                    myLag.append(temp);
                }
                else {
                    res.append(c + tab + output[1] + "\n");
                    myLag.append(c);
                }
            } else {
                res.append(c + tab + output[1] + "\n");
                myLag.append(c);
            }
            begin = removeBlank(sentence, begin + 1, len) - 1;
        } else if (sentence[begin] != ' ') {
            isNum = false;
            if ((sentence[begin] == 'e'||sentence[begin] == 'E')&& !word.isEmpty()&& judge(word) == 3)
                isNum = true;

            word += sentence[begin];
            if (sentence[begin] == '"') {

                if (begin == 0)
                    isString = !isString;
                else if (begin > 0 && sentence[begin - 1] != '\\')
                    isString = !isString;
            }

        } else if(sentence[begin] == ' ') {
            if (isString) {
                isNum = false;
                word += sentence[begin];
            }

            else {
                if (word.isEmpty()) {
                    begin++;
                    continue;
                }

                int res_ = judge(word);
                res.append(word + tab + output[res_] + "\n");
                if (res_ == 0) {
                    myLag.append(word.toUpper());
                } else {
                    myLag.append(word);
                }
                word = "";
                begin = removeBlank(sentence, begin, len) - 1;
            }
        }

        begin++;
    }
    if (!word.isEmpty()) {
        res.append(word + "\n");
        myLag.append(word);
        word = "";
    }
    myLag.append("\n");
}

void division::getMyLanguage(QTextEdit * resText)
{
    resText->setText(myLag);
}
