#ifndef GRAPH_H
#define GRAPH_H
#include <QString>

struct Edge
{
    int dest;
    QChar character;
    Edge * next;
    Edge() {
        next = nullptr;
    }
    Edge(int dest_,QChar chr, Edge * next_):dest(dest_),character(chr),next(next_){}
};

struct Vertex {
    QString data;
    Edge * adj = nullptr;
    bool isEnd;
};

class graph
{
public:
    graph();
    bool insertEdge(const QChar character, int start, int end);
    bool insertVertex();
    bool insertVertex(QString data, bool isEnd = false);
    int getVertexSize() const;
    Vertex * getNodeTable() {return nodeTable;}
    int findIndexByData(QString data);

private:
    Vertex * nodeTable;
    int vertexSize;
    int edgeSize;
    int maxSize = 120;
};

#endif // GRAPH_H
