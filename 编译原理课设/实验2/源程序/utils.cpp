#include "utils.h"

utils::utils()
{
   dotPath = "";
   dotPath.append(QCoreApplication::applicationDirPath() + "/Graphviz//bin/dot.exe");
}


int utils::getPriority(QChar c)
{
    if (c == '*' || c == '+' || c == '?') {
        return  3;
    } else if (c == '&') {
        return 2;
    } else if (c == '|') {
        return 1;
    } else {
        return 0;
    }
}

QString utils::changeSymbol(QString regEx)
{
    int len = regEx.length();
    QString res = "";
    for (int t = 0; t < len; t++) {
        QChar cur = regEx[t];
        if (cur == '[') {
            bool flag = false;
            res.append('(');
            t = t + 1;
            while (t < len && regEx[t] != ']') {
                if (flag == true)
                    res.append('|');
                res.append(regEx[t]);
                if (t + 1 < len && t + 2 < len && regEx[t + 1] == '-') {
                    flag = true;
                    QChar tmp = QChar(regEx[t].unicode() + 1);

                    while (tmp <= regEx[t + 2]) {
                        res.append("|");
                        res.append(tmp);
                        tmp = QChar(tmp.unicode() + 1);
                    }
                    t = t + 2;
                } else if (t + 2 < len)
                    res.append('|');
                t = t + 1;
            }
            res.append(')');
        } else {
            res.append(cur);
        }
    }
    return res;
}

QString utils::addSymbol(QString regEx)
{
    int len = regEx.length();
    QString res = "";
    for (int t = 0; t < len - 1; t++) {
        QChar cur = regEx[t];
        QChar next = regEx[t+1];
        res.append(cur);
        if (cur != '(' && next != ')' && cur !='|' && next != '|' &&
                next != '*' && next != '+' && next != '?') {
            res.append('&');
        }
    }
    res.append(regEx[len - 1]);
    return res;
}

QString utils::getPostOrderEx(QString regEx)
{
    regEx = changeSymbol(regEx);
    regEx = addSymbol(regEx);
    QStack<QChar> stack;
    QString res = "";
    int len = regEx.length();
    for (int t = 0;t < len; t++) {
        if ((regEx[t] >= '0' && regEx[t] <= '9') || (regEx[t] >= 'a' && regEx[t] <= 'z') || (regEx[t] >= 'A' && regEx[t] <= 'Z')) {
            res.push_back(regEx[t]);
        } else {
            if (regEx[t] == '(')
                stack.push(regEx[t]);
            else if (regEx[t] == ')') {
                while (stack.top() != '(') {
                    res += stack.pop();
                }
                stack.pop();
            } else {
                if (!stack.isEmpty()) {
                    QChar c = stack.top();
                    if (c == '(')
                        stack.push(regEx[t]);
                    else if (getPriority(regEx[t]) > getPriority(c))
                        stack.push(regEx[t]);
                    else {
                        while (!stack.isEmpty() && stack.top() != '(') {
                            QChar c = stack.top();
                            if (getPriority(regEx[t]) <= getPriority(c))
                                res+=stack.pop();
                            else
                                break;
                        }
                        stack.push(regEx[t]);
                    }
                } else {
                    stack.push(regEx[t]);
                }
            }
        }
    }
    while (!stack.isEmpty())
        res += stack.pop();
    return res;
}

void utils::basic(QChar c)
{
    int vertexSize = nfaGraph.getVertexSize();
    nfaGraph.insertVertex();
    nfaGraph.insertVertex();
    stack.push(vertexSize);
    stack.push(vertexSize + 1);
    nfaGraph.insertEdge(c, vertexSize, vertexSize + 1);
}

void utils::apposition()
{
    int rightEnd = stack.pop();
    int rightStart = stack.pop();
    int leftEnd = stack.pop();
    int leftStart = stack.pop();
    nfaGraph.insertEdge(epsilon, leftEnd, rightStart);
    stack.push(leftStart);
    stack.push(rightEnd);
}

void utils::selection()
{
    int secondEnd = stack.pop();
    int secondStart = stack.pop();
    int firstEnd = stack.pop();
    int firstStart = stack.pop();
    int vertexSize = nfaGraph.getVertexSize();
    nfaGraph.insertVertex();
    nfaGraph.insertVertex();
    nfaGraph.insertEdge(epsilon, vertexSize, firstStart);
    nfaGraph.insertEdge(epsilon, vertexSize, secondStart);
    nfaGraph.insertEdge(epsilon, firstEnd, vertexSize + 1);
    nfaGraph.insertEdge(epsilon, secondEnd, vertexSize + 1);
    stack.push(vertexSize);
    stack.push(vertexSize + 1);
}

void utils::closure()
{
    int end = stack.pop();
    int start = stack.pop();
    int vertexSize = nfaGraph.getVertexSize();
    nfaGraph.insertVertex();
    nfaGraph.insertVertex();
    nfaGraph.insertEdge(epsilon, end, start);
    nfaGraph.insertEdge(epsilon, end, vertexSize + 1);
    nfaGraph.insertEdge(epsilon, vertexSize, start);
    nfaGraph.insertEdge(epsilon, vertexSize, vertexSize + 1);
    stack.push(vertexSize);
    stack.push(vertexSize + 1);
}

void utils::optional()
{
    int end = stack.pop();
    int start = stack.pop();
    int vertexSize = nfaGraph.getVertexSize();
    nfaGraph.insertVertex();
    nfaGraph.insertVertex();
    nfaGraph.insertEdge(epsilon, vertexSize, start);
    nfaGraph.insertEdge(epsilon, vertexSize, vertexSize + 1);
    nfaGraph.insertEdge(epsilon, end, vertexSize + 1);
    stack.push(vertexSize);
    stack.push(vertexSize + 1);
}

void utils::positiveClosure()
{
    int end = stack.pop();
    int start = stack.pop();
    int vertexSize = nfaGraph.getVertexSize();
    nfaGraph.insertVertex();
    nfaGraph.insertVertex();
    nfaGraph.insertEdge(epsilon, end, start);
    nfaGraph.insertEdge(epsilon, end, vertexSize + 1);
    nfaGraph.insertEdge(epsilon, vertexSize, start);
    stack.push(vertexSize);
    stack.push(vertexSize + 1);
}

void utils::getNfa(QString postEx)
{
    postEx = getPostOrderEx(postEx);
    charSet.insert(QChar(0x03B5));
    int len = postEx.length();
    for (int t = 0; t < len; t++) {
        if ((postEx[t] >= '0' && postEx[t] <= '9') || (postEx[t] >= 'a' && postEx[t] <= 'z') || (postEx[t] >= 'A' && postEx[t] <= 'Z')) {
            QChar c = postEx[t];
            basic(c);
            charSet.insert(postEx[t]);
        } else if (postEx[t] == '&') {
            apposition();
        } else if (postEx[t] == '|') {
            selection();
        } else if (postEx[t] == '*') {
            closure();
        } else if (postEx[t] == '?') {
            optional();
        } else {
            positiveClosure();
        }
    }
    int end = stack.pop();
    int start = stack.pop();
    nfaEndNode = end;
    nfaStartNode = start;
    stack.push(start);
    stack.push(end);
}

void utils::drawNfa(QTableWidget *table, QLabel * label)
{
    showTable(table, nfaGraph);
    QString curPath = QCoreApplication::applicationDirPath();
    QString nfaTxtPath = curPath + "/nfa.txt";
    QFile nfaTxt(nfaTxtPath);

    if (nfaTxt.open(QFile::WriteOnly|QFile::Truncate)) {
        QTextStream out(&nfaTxt);
        out << "digraph nfa{\n";
        out << "rankdir=LR;\n";
        out << "charset=\"utf-8\";\n";
        out << nfaGraph.getNodeTable()[nfaStartNode].data << "[fontcolor=orange];\n";
        out << nfaGraph.getNodeTable()[nfaEndNode].data << "[fontcolor=black][shape=doublecircle];\n";


        QSet<int> qset;
        QStack<int> s;
        s.push(nfaStartNode);
        qset.insert(nfaStartNode);
        while (!s.isEmpty()) {
            int curStartNode = s.pop();
            Edge * curEdge = nfaGraph.getNodeTable()[curStartNode].adj;
            while (curEdge != nullptr) {
                if (curEdge->character != epsilon)
                    out << nfaGraph.getNodeTable()[curStartNode].data << "->"
                        << curEdge->dest<<"[label=" << QString(curEdge->character) << "];\n";
                else
                    out << nfaGraph.getNodeTable()[curStartNode].data << "->"
                        << curEdge->dest<<"[label=" << "\"&epsilon;\"" << "];\n";
                if (qset.find(curEdge->dest) == qset.end()) {
                    s.push(curEdge->dest);
                    qset.insert(curEdge->dest);
                }

                curEdge = curEdge->next;
            }
        }
        out << "}";
        nfaTxt.close();
    }

    QStringList args;
    args << nfaTxtPath << "-Tpng" << "-o" << curPath + "/nfa.png";
    QProcess *process = new QProcess;
    process->execute(dotPath, args);
    QImage image(curPath + "/nfa.png");
    label->clear();
    label->setPixmap(QPixmap::fromImage(image));
    label->show();
}

QString utils::setToQStringAndJudgeWhetherEnd(QSet<int> &set, bool &isEnd)
{
    isEnd = false;
    QString res = "{";
    for (auto it = set.begin(); it != set.end(); it++) {
        if (*it == nfaEndNode) {
            isEnd = true;
        }
        if (res != "{")
            res = res + "," + QString::number(*it);
        else {
            res = res + QString::number(*it);
        }
    }
    res += "}";
    return res;
}


void utils::epsilonClosure(int start, QSet<int> &set)
{
    set.insert(start);
    Vertex * nodeTable = nfaGraph.getNodeTable();
    Edge *cur = nodeTable[start].adj;
    while (cur != nullptr) {
        if (cur->character == epsilon) {
            auto iterator = set.find(cur->dest);
            if (iterator == set.end()) {
                set.insert(cur->dest);
                epsilonClosure(cur->dest, set);
            }
        }
        cur = cur->next;
    }
}


void utils::getDfa()
{
    charSet.remove(QChar(0x03B5));
    bool isEnd1;    //"begin"是否为end
    bool isEnd2;    //"end"是否为end
    Vertex * nodeTable = nfaGraph.getNodeTable();

    QSet<int> set;
    epsilonClosure(nfaStartNode, set);

    QStack<QSet<int> > stack;
    QSet<QString>state;  //防止多次判断
    stack.push(set);
    QString begin = setToQStringAndJudgeWhetherEnd(set, isEnd1);
    dfaGraph.insertVertex(begin, isEnd1);
    while (! stack.empty()) {
        QSet<int> cur = stack.pop();
        QString begin = setToQStringAndJudgeWhetherEnd(cur, isEnd1);
        if (state.find(begin) != state.end())
            continue;
        else {
            state.insert(begin);
        }
        int beginIndex = dfaGraph.findIndexByData(begin);
        for (auto it = charSet.begin(); it != charSet.end(); it++) {
            QChar curChar = *it;

            QSet<int> tmp;
            for (auto i = cur.begin(); i != cur.end(); i++) {
                Edge * edge = nodeTable[*i].adj;
                while (edge != nullptr) {
                    if (edge->character == curChar && tmp.find(edge->dest) == tmp.end()) {
                        epsilonClosure(edge->dest, tmp);
                    }
                    edge = edge->next;
                }
            }
            if (tmp.size() != 0) {
                QString end = setToQStringAndJudgeWhetherEnd(tmp, isEnd2);
                int endIndex = dfaGraph.findIndexByData(end);
                if (endIndex == -1) {
                    int vertexSize = dfaGraph.getVertexSize();
                    endIndex = vertexSize;
                    dfaGraph.insertVertex(end, isEnd2);
                }
                stack.push(tmp);
                dfaGraph.insertEdge(curChar, beginIndex, endIndex);
            }

        }
    }
}


void utils::drawDfa(QTableWidget *table, QLabel *label)
{
    showTable(table, dfaGraph);
    QString curPath = QCoreApplication::applicationDirPath();
    QString dfaTxtPath = curPath + "/dfa.txt";
    QFile dfaTxt(dfaTxtPath);
    if (dfaTxt.open(QFile::WriteOnly|QFile::Truncate)) {
        QTextStream out(&dfaTxt);
        out << "digraph dfa{\n";
        out << "rankdir=LR;\n";
        //因为在dfa图中第一个结点肯定是第一插入
        out <<"\"" << dfaGraph.getNodeTable()[1].data << "\"" << "[fontcolor=orange];\n";
        //out << nfaGraph.getNodeTable()[nfaEndNode].data << "[fontcolor=black][label=end]\n;";


        QStack<int> s;
        QSet<int> qset;
        s.push(1);
        qset.insert(1);
        while (!s.isEmpty()) {
            int curStartNode = s.pop();
            if (dfaGraph.getNodeTable()[curStartNode].isEnd) {
                if (curStartNode != 1)
                    out << "\"" << dfaGraph.getNodeTable()[curStartNode].data << "\"" <<"[fontcolor=black][shape=doublecircle];"<<endl;
                else {
                    out << "\"" << dfaGraph.getNodeTable()[curStartNode].data << "\"" <<"[fontcolor=orange][shape=doublecircle];"<<endl;
                }
            }

            Edge * curEdge = dfaGraph.getNodeTable()[curStartNode].adj;
            while (curEdge != nullptr) {
                out << "\"" << dfaGraph.getNodeTable()[curStartNode].data  << "\"" << "->"
                    << "\"" << dfaGraph.getNodeTable()[curEdge->dest].data << "\""
                    <<"[label=" << QString(curEdge->character) << "];\n";
                if (qset.find(curEdge->dest) == qset.end()) {
                    s.push(curEdge->dest);
                    qset.insert(curEdge->dest);
                }
                curEdge = curEdge->next;
            }
        }
        out << "}";
        dfaTxt.close();
    }

    QStringList args;
    args << dfaTxtPath << "-Tpng" << "-o" << curPath + "/dfa.png";
    QProcess *process = new QProcess;
    process->execute(dotPath, args);
    QImage image(curPath + "/dfa.png");
    label->clear();
    label->setPixmap(QPixmap::fromImage(image));
    label->show();
}

void utils::minimizeDfa()
{
    //用于表示经过某条表是否到达终态, false表示不是,
    typedef QPair<QChar, bool> p;
    //用于存储dfa各个顶点经过各个边后的状态
    QMap<QString, QVector<p> > map;

    Vertex * dfaNodeTable = dfaGraph.getNodeTable();
    int vertexSize = dfaGraph.getVertexSize();

    for (int t = 1; t< vertexSize; t++) {
        QString data = dfaNodeTable[t].data;
        map[data] = QVector<p> ();
        Edge * edge = dfaNodeTable[t].adj;
        while (edge != nullptr) {
            int dest = edge->dest;
             map[data].push_back(p(edge->character, dfaNodeTable[dest].isEnd));
             edge = edge->next;
        }
    }

    for (auto it = map.begin(); it != map.end(); it++) {
        QString key = it.key();
        QVector<p> vec = map[key];
        if (vec.size() != 0) {
           std::sort(vec.begin(), vec.end(), [](p a, p b)->bool{return a.first < b.first;});
           map[key] = vec;
        }
    }

    QVector<QSet<QString>> group;
    QSet<QString> exist;
    for (auto it = map.begin(); it != map.end(); it++) {
        QString key = it.key();
        if (exist.find(key) != exist.end())
            continue;
        else {
            exist.insert(key);
        }
        int beginIndex = dfaGraph.findIndexByData(key);
        bool isEnd1 = dfaGraph.getNodeTable()[beginIndex].isEnd;
        QVector<p> vec1 = map[key];

        QSet<QString> set;
        set.insert(key);
        for (auto it2 = it + 1; it2 != map.end(); it2++) {
            QString curKey = it2.key();
            QVector<p> vec2 = map[curKey];
            if (vec1.size() != vec2.size())
                continue;
            else {
                int endIndex = dfaGraph.findIndexByData(curKey);
                bool isEnd2 = dfaGraph.getNodeTable()[endIndex].isEnd;
                if (isEnd1 != isEnd2)
                    continue;
                int t = 0;
                bool flag = true;
                for (t = 0; t < vec1.size(); t++) {
                    if (vec1[t].first != vec2[t].first || vec1[t].second != vec2[t].second) {
                        flag = false;
                        break;
                    }
                }
                if (!flag)
                    continue;
                else {
                    exist.insert(curKey);
                    set.insert(curKey);
                }
            }
        }
        group.push_back(set);
    }

    int groupSize = group.size();
    for (int t = 1; t < vertexSize; t++) {
        QString data = dfaNodeTable[t].data;
        bool isEnd1 = dfaNodeTable[t].isEnd;
        //startIndex作为存入minidfa的data
        int startIndex;

        for (startIndex = 0;startIndex < groupSize;startIndex++) {
            QSet<QString> set = group[startIndex];
            if (set.find(data) != set.end())
                break;
        }
        int graphStartIndex = miniDfaGraph.findIndexByData(QString::number(startIndex + 1));
        if (graphStartIndex == -1) {
            graphStartIndex = miniDfaGraph.getVertexSize();
            miniDfaGraph.insertVertex(QString::number(startIndex + 1), isEnd1);

        }

        Edge *edge = dfaNodeTable[t].adj;
        while (edge != nullptr) {
            int dst = edge->dest;
            QString dstData = dfaNodeTable[dst].data;
            bool isEnd2 = dfaNodeTable[dst].isEnd;
            int endIndex;
            for (endIndex = 0; endIndex < groupSize;endIndex++) {
                QSet<QString> set = group[endIndex];
                if (set.find(dstData) != set.end())
                    break;
            }
            int graphEndIndex = miniDfaGraph.findIndexByData(QString::number(endIndex + 1));
            if (graphEndIndex == -1) {
                graphEndIndex = miniDfaGraph.getVertexSize();
                miniDfaGraph.insertVertex(QString::number(endIndex + 1), isEnd2);
            }
            miniDfaGraph.insertEdge(edge->character, graphStartIndex, graphEndIndex);
            edge = edge->next;
        }


    }
}

void utils::drawminiDfa(QTableWidget *table, QLabel *label)
{
    showTable(table, miniDfaGraph);
    QString curPath = QCoreApplication::applicationDirPath();
    QString minidfaTxtPath = curPath + "/minidfa.txt";
    QFile minidfaTxt(minidfaTxtPath);
    if (minidfaTxt.open(QFile::WriteOnly|QFile::Truncate)) {
        QTextStream out(&minidfaTxt);
        out << "digraph minidfa{\n";
        out << "rankdir=LR;\n";
        //因为在dfa图中第一个结点肯定是第一插入
        out <<"\"" << miniDfaGraph.getNodeTable()[1].data << "\"" << "[fontcolor=orange];\n";
        int startIndex = 1;

        QStack<int> s;
        QSet<int> qset;
        qset.insert(startIndex);
        s.push(startIndex);
        while (!s.isEmpty()) {
            int curStartNode = s.pop();
            if (miniDfaGraph.getNodeTable()[curStartNode].isEnd) {
                if (curStartNode != startIndex)
                    out << "\"" << miniDfaGraph.getNodeTable()[curStartNode].data << "\"" <<"[fontcolor=black][shape=doublecircle];"<<endl;
                else {
                    out << "\"" << miniDfaGraph.getNodeTable()[curStartNode].data << "\"" <<"[fontcolor=orange][shape=doublecircle];"<<endl;
                }
            }
            Edge * curEdge = miniDfaGraph.getNodeTable()[curStartNode].adj;
            while (curEdge != nullptr) {
                out << miniDfaGraph.getNodeTable()[curStartNode].data << "->"
                    << miniDfaGraph.getNodeTable()[curEdge->dest].data
                    <<"[label=" << QString(curEdge->character) << "];\n";
                if (qset.find(curEdge->dest) == qset.end()) {
                    s.push(curEdge->dest);
                    qset.insert(curEdge->dest);
                }
                curEdge = curEdge->next;
            }
        }
        out << "}";
        minidfaTxt.close();
    }

    QStringList args;
    args << minidfaTxtPath << "-Tpng" << "-o" << curPath + "/minidfa.png";
    QProcess *process = new QProcess;
    process->execute(dotPath, args);
    QImage image(curPath + "/minidfa.png");
    label->clear();
    label->setPixmap(QPixmap::fromImage(image));
    label->show();
}

QString utils::getCode()
{
    QString res = "";
    //必要的头文件
    res.append("#include<iostream> \n");
    res.append("bool isRight(string s)\n");
    res.append("{\n");
    res.append("\tint len = s.length();\n");
    res.append("\tint t = 0;\n");

    QString beginData = dfaGraph.getNodeTable()[1].data;
    int totalState = miniDfaGraph.getVertexSize();
    res.append("\tint state = 1;\n");

    res.append("\twhile (t < len && (");
    for (int t = 1; t < totalState -1; t++) {
        res.append("state == " + QString::number(t) + "||");
    }
    res.append("state == " + QString::number(totalState - 1) + ") ) {\n");
    res.append("\t\tchar cur = s[t];\n");
    res.append("\t\tbool flag = false;\n");
    res.append("\t\tswitch(state) {\n");
    for (int t = 1; t < totalState; t++) {
        res.append("\t\tcase " + QString::number(t) + ":\n");

        Edge *edge = miniDfaGraph.getNodeTable()[t].adj;
        while (edge != nullptr) {
            int dst = edge->dest;
            QChar ch = edge->character;
            res.append("\t\t\tif(cur == ");
            res.append(ch);
            res.append(") {\n");
            res.append("\t\t\t\t");
            res.append("state = " + QString::number(dst) + ";\n");
            res.append("\t\t\t\t flag = true;\n");
            res.append("\t\t\t}\n");
            edge = edge->next;
        }

        res.append("\t\t\tbreak;\n");
    }
        res.append("\t\t}\n");
    res.append("\t\tif (flag) {\n");
    res.append("\t\t\t t = t + 1;\n");
    res.append("\t\t} else {\n");
    res.append("\t\t\t state = " + QString::number(totalState) + ";\n");
    res.append("\t\t}\n");
    res.append("\t}\n");
    QVector<int> vec;
    for (int t = 1; t< totalState; t++) {
        if (miniDfaGraph.getNodeTable()[t].isEnd)
            vec.push_back(t);
    }
    res.append("\tif ((");
    for (int t = 0;t < vec.size() - 1; t++) {
        res.append("state == " + QString::number(vec[t]) + "||");
    }
    res.append("state == " + QString::number(vec[vec.size() - 1]) + ") && t == len) { \n");
    res.append("\t\treturn true;\n");
    res.append("\t} else {\n");
    res.append("\t\treturn false;\n");
    res.append("\t}\n");
    res.append("}");
    return res;
}

void utils::showTable(QTableWidget *table, graph graph)
{
    table->clear();
    table->setFont(QFont("Times New Roman"));
    int columnCount = charSet.size();
    int rowCount = graph.getVertexSize();
    table->setColumnCount(columnCount);
    table->setRowCount(rowCount - 1);
    QList<QChar> charList = charSet.toList();
    std::sort(charList.begin(), charList.end());

    QMap<QChar, int> map;
    QStringList header;
    for (int t = 0; t < charList.length(); t++) {
        map[charList[t]] = t;
        header << charList[t];
    }
    table->setHorizontalHeaderLabels(header);

    int vertexSize = graph.getVertexSize();
    Vertex * nodeTable = graph.getNodeTable();
    for (int t = 1; t < vertexSize; t++) {
        Edge * edge = nodeTable[t].adj;
        while (edge != nullptr) {
            QChar ch = edge->character;
            int col = map[ch];
            QTableWidgetItem * item = table->item(t-1, col);
            if (item == nullptr) {
                table->setItem(t-1, col, new QTableWidgetItem(QString::number(edge->dest)));
            } else {
                QString preData = item->text();
                table->setItem(t-1, col, new QTableWidgetItem(QString::number(edge->dest) + ", " + preData));
            }
            edge = edge->next;
        }
    }
    table->show();
}
