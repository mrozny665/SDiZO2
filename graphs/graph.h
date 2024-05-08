#ifndef SDIZO2_GRAPH_H
#define SDIZO2_GRAPH_H


#include <string>
#include "matrix.h"
#include "list.h"
#include "util/edge.h"

class Graph {
    int *nodes{};
    Edge *edges{};
public:
    Matrix matrix;
    AdjList list;
    int nodeCount{}, edgeCount{}, startNode, endNode;
    void loadFromFile(const std::string& fileName, int problem);
    void generateUndirected(int nodes1, int density);
    void generateDirected(int nodes1, int density);
    void display() const;
    void primAlgorithm(int startingNode);
    void kruskalAlgorithm();
    void dijkstraAlgorithm();
    void bellmanFordAlgorithm();

    void primTest(int nodes1, int density);
    double kruskalTest(int nodes1, int density);
    double dijkstraTest(int nodes1, int density);
    double bellmanFordTest(int nodes1, int density);

    Graph();
};


#endif //SDIZO2_GRAPH_H
