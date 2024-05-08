#ifndef SDIZO2_MATRIX_H
#define SDIZO2_MATRIX_H


#include "util/edge.h"

class Matrix {
public:

    int **matrix{};
    int nodeCount{};
    int edgeCount{};
    bool directed{};
    int startNode{};
    int endNode{};

    Matrix();
    Matrix(int nodes, int edges, bool directed);
    Matrix(int nodes, int edges, bool directed, int startNode);
    Matrix(int nodes, int edges, bool directed, int startNode, int endNode);
    ~Matrix();

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


#endif //SDIZO2_MATRIX_H
