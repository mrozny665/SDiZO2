#ifndef SDIZO2_LIST_H
#define SDIZO2_LIST_H


#include "../structures/list.h"
#include "util/edge.h"

class AdjList {
public:

    List *list{};
    int nodeCount{};
    int edgeCount{};
    bool directed{};
    int startNode{};
    int endNode{};

    AdjList();
    AdjList(int nodes, int edges, bool directed);
    AdjList(int nodes, int edges, bool directed, int startNode);
    AdjList(int nodes, int edges, bool directed, int startNode, int endNode);
    ~AdjList();

    void generateUndirected(Edge *edges) const;
    void generateDirected(Edge *edges) const;
    void primAlgorithm(int startingNode);
    void kruskalAlgorithm();
    void dijkstraAlgorithm();
    void bellmanFordAlgorithm();

    void display() const;

    int FindSet(int x, int *parent);
    void Union(int x, int y, int *parent, int *rank);

    double primTest(int startingNode);
    double kruskalTest();
    double dijkstraTest();
    double bellmanFordTest();
};


#endif //SDIZO2_LIST_H
