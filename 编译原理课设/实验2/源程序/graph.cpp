#include "graph.h"

graph::graph()
{
    nodeTable = new Vertex[maxSize + 5];
    vertexSize = 1;
    edgeSize = 0;
}

bool graph::insertEdge(const QChar character, int start, int end)
{
    Edge * adj = nodeTable[start].adj;
    bool flag = true;
    while (adj != nullptr) {
        if (adj->character == character && adj->dest == end) {
            flag = false;
            break;
        }
        adj = adj->next;
    }
    if (!flag)
        return true;
    Edge * cur = new Edge(end, character, nodeTable[start].adj);
    nodeTable[start].adj = cur;
    edgeSize++;
    return true;
}

bool graph::insertVertex()
{
    nodeTable[vertexSize].data = QString::number(vertexSize);
    vertexSize++;
    return true;
}

bool graph::insertVertex(QString data, bool isEnd)
{
    nodeTable[vertexSize].data = data;
    nodeTable[vertexSize].isEnd = isEnd;
    vertexSize++;
    return true;
}

int graph::getVertexSize() const
{
    return vertexSize;
}

int graph::findIndexByData(QString data)
{
    for (int t = 1;t < vertexSize + 1; t++) {
        if (nodeTable[t].data == data)
            return t;

    }
    return -1;
}
