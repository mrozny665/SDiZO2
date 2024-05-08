#include "graph.h"
#include<fstream>
#include<iostream>

void Graph::loadFromFile(const std::string& fileName, int problem) {
    std::ifstream file(fileName);
    file >> edgeCount >> nodeCount;
    if (problem == 1){
        file >> startNode;
        matrix = Matrix(nodeCount, edgeCount, true, startNode);
        list = AdjList(nodeCount, edgeCount, true, startNode);
    } else if (problem == 2){
        file >> startNode >> endNode;
        matrix = Matrix(nodeCount, edgeCount, true, startNode, endNode);
        list = AdjList(nodeCount, edgeCount, true, startNode, endNode);
    } else {
        matrix = Matrix(nodeCount, edgeCount, false);
        list = AdjList(nodeCount, edgeCount, false);
    }
    edges = new Edge[edgeCount];
    for (int i = 0; i < edgeCount; i++){
        int node1, node2, weight;
        file >> node1 >> node2 >> weight;
        Edge newEdge = Edge{node1, node2, weight};
        edges[i] = newEdge;
    }
    if (problem == 1 || problem == 2) {
        matrix.generateDirected(edges);
        list.generateDirected(edges);
    } else {
        matrix.generateUndirected(edges);
        list.generateUndirected(edges);
    }
}

void Graph::generateUndirected(int nodes1, int density) {
    srand((unsigned int)(size_t) time);
    nodeCount = nodes1;
    edgeCount = (nodeCount * (nodeCount - 1)) / 2;
    float fl_edgeCount = (float) edgeCount * ((float) density / 100);
    edgeCount = (int) fl_edgeCount;
    if (density <= 50) {
        int a = nodeCount;
        int notAdded[nodeCount];
        for (int i = 0; i < nodeCount; i++) {
            notAdded[i] = i;
        }
        nodes = new int[nodeCount];
        edges = new Edge[edgeCount];
        nodes[0] = rand() % nodeCount;
        notAdded[nodes[0]] = notAdded[a - 1];
        int edgesToAdd = edgeCount;
        a--;
        int i = 1;
        for (; i < nodeCount; i++) { //dodawanie krawędzi tworzących drzewo rozpinające
            int b = rand() % a;
            int toAdd = notAdded[b];
            nodes[i] = toAdd;
            notAdded[b] = notAdded[a - 1];
            a--;
            int weight = (rand() % 7) + 1;
            Edge newEdge{nodes[i - 1], nodes[i], weight};
            edges[i - 1] = newEdge;
            edgesToAdd--;
        }
        i--;
        for (; i < edgeCount; i++) { //dodawanie pozostałych krawędzi do grafu
            bool repeat;
            int b;
            do {
                repeat = false;
                a = rand() % nodeCount;
                do {
                    b = rand() % nodeCount;
                } while (b == a);
                for (int j = 0; j < i; j++) {
                    if ((a == edges[j].node1 || a == edges[j].node2) &&
                        (b == edges[j].node1 || b == edges[j].node2))
                        repeat = true;
                }
            } while (repeat);
            int weight = (rand() % 7) + 1;
            Edge newEdge{a, b, weight};
            edges[i] = newEdge;
        }
    } else {
        int allEdgeCount = (nodeCount * (nodeCount-1))/2;
        Edge newEdges[allEdgeCount];
        edges = new Edge[edgeCount];
        int counter = 0;
        for (int i = 0; i < nodeCount; i++){
            for (int j = i; j < nodeCount; j++){
                if (i != j) {
                    int weight = (rand() % 7) + 1;
                    Edge newEdge{i, j, weight};
                    newEdges[counter++] = newEdge;
                }
            }
        }
        int edgeDiff = allEdgeCount - edgeCount;
        for (int i = 0; i < edgeDiff; i++){
            int toSwap = rand() % (allEdgeCount-i);
            newEdges[toSwap] = newEdges[allEdgeCount-1-i];
        }
        for (int i = 0; i < edgeCount; i++){
            edges[i] = newEdges[i];
        }
    }
    matrix = Matrix(nodeCount, edgeCount, false);
    list = AdjList(nodeCount, edgeCount, false);
    matrix.generateUndirected(edges); //tworzenie macierzy na podstawie listy krawędzi
    list.generateUndirected(edges); //tworzenie listy sąsiedztwa na podstawie listy krawędzi
}

void Graph::generateDirected(int nodes1, int density) {
    srand((unsigned int)(size_t) time);
    nodeCount = nodes1;
    edgeCount = (nodeCount * (nodeCount - 1));
    float fl_edgeCount = (float) edgeCount * ((float) density / 100);
    edgeCount = (int) fl_edgeCount;
    if (density <= 50) {
        int a = nodeCount;
        int notAdded[nodeCount];
        for (int i = 0; i < nodeCount; i++) {
            notAdded[i] = i;
        }
        nodes = new int[nodeCount];
        edges = new Edge[edgeCount];
        nodes[0] = rand() % nodeCount;
        notAdded[nodes[0]] = notAdded[a - 1];
        int edgesToAdd = edgeCount;
        a--;
        int i = 1;
        for (; i < nodeCount; i++) { //dodawanie krawędzi tworzących drzewo rozpinające
            int b = rand() % a;
            int toAdd = notAdded[b];
            nodes[i] = toAdd;
            notAdded[b] = notAdded[a - 1];
            a--;
            int weight = (rand() % 7) + 1;
            Edge newEdge{nodes[i - 1], nodes[i], weight};
            edges[i - 1] = newEdge;
            edgesToAdd--;
        }
        i -= 2;
        int weight = (rand() % 7) + 1;
        Edge newEdge{nodes[i - 1], nodes[0], weight};
        edges[i - 1] = newEdge;
        i++;
        for (; i < edgeCount; i++) { //dodawanie pozostałych krawędzi do grafu
            bool repeat;
            int b;
            do {
                repeat = false;
                a = rand() % nodeCount;
                do {
                    b = rand() % nodeCount;
                } while (b == a);
                for (int j = 0; j < i; j++) {
                    if ((a == edges[j].node1 || a == edges[j].node2) &&
                        (b == edges[j].node1 || b == edges[j].node2))
                        repeat = true;
                }
            } while (repeat);
            weight = (rand() % 7) + 1;
            Edge newEdge{a, b, weight};
            edges[i] = newEdge;
        }
    } else {
        int allEdgeCount = nodeCount * (nodeCount-1);
        Edge newEdges[allEdgeCount];
        edges = new Edge[edgeCount];
        int counter = 0;
        for (int i = 0; i < nodeCount; i++){
            for (int j = 0; j < nodeCount; j++){
                if (i != j) {
                    int weight = (rand() % 7) + 1;
                    Edge newEdge{i, j, weight};
                    newEdges[counter++] = newEdge;
                }
            }
        }
        int edgeDiff = allEdgeCount - edgeCount;
        for (int i = 0; i < edgeDiff; i++){
            int toSwap = rand() % (allEdgeCount-i);
            newEdges[toSwap] = newEdges[allEdgeCount-1-i];
        }
        for (int i = 0; i < edgeCount; i++){
            edges[i] = newEdges[i];
        }
    }
    startNode = rand() % nodeCount;
    endNode = rand() % nodeCount;
    matrix = Matrix(nodeCount, edgeCount, true, startNode, endNode);
    list = AdjList(nodeCount, edgeCount, true, startNode, endNode);
    matrix.generateDirected(edges); //tworzenie macierzy na podstawie listy krawędzi
    list.generateDirected(edges); //tworzenie listy sąsiedztwa na podstawie listy krawędzi
}

void Graph::display() const {
    matrix.display();
    list.display();
}

void Graph::primAlgorithm(int startingNode) {
    if (startingNode == -1) {
        srand((unsigned int) (size_t) time);
        startingNode = rand() % nodeCount;
    }
    matrix.primAlgorithm(startingNode);
    list.primAlgorithm(startingNode);
}

void Graph::kruskalAlgorithm() {
    matrix.kruskalAlgorithm();
    list.kruskalAlgorithm();
}

void Graph::dijkstraAlgorithm() {
    matrix.dijkstraAlgorithm();
    list.dijkstraAlgorithm();
}

void Graph::bellmanFordAlgorithm() {
    matrix.bellmanFordAlgorithm();
    list.bellmanFordAlgorithm();
}

void Graph::primTest(int nodes1, int density) {
    double timeMatrix = 0;
    double timeList = 0;
    for (int i = 0; i < 50; i++) {
        generateUndirected(nodes1, density);
        startNode = rand() % nodes1;
        timeMatrix += matrix.primTest(startNode);
        timeList += list.primTest(startNode);
    }
    timeMatrix /= 50;
    timeList /= 50;
    std::cout << "Prim " << nodes1 << " | " << density << " || " << timeMatrix << " | " << timeList << std::endl;
}

double Graph::kruskalTest(int nodes1, int density) {
    double timeMatrix = 0;
    double timeList = 0;
    for (int i = 0; i < 50; i++) {
        generateUndirected(nodes1, density);
        timeMatrix += matrix.kruskalTest();
        timeList += list.kruskalTest();
    }
    timeMatrix /= 50;
    timeList /= 50;
    std::cout << "Kruskal " << nodes1 << " | " << density << " || " << timeMatrix << " | " << timeList << std::endl;
}

double Graph::dijkstraTest(int nodes1, int density) {
    double timeMatrix = 0;
    double timeList = 0;
    for (int i = 0; i < 50; i++) {
        generateDirected(nodes1, density);
        timeMatrix += matrix.dijkstraTest();
        timeList += list.dijkstraTest();
    }
    timeMatrix /= 50;
    timeList /= 50;
    std::cout << "Dijkstra " << nodes1 << " | " << density << " || " << timeMatrix << " | " << timeList << std::endl;
}

double Graph::bellmanFordTest(int nodes1, int density) {
    double timeMatrix = 0;
    double timeList = 0;
    for (int i = 0; i < 50; i++) {
        generateDirected(nodes1, density);
        timeMatrix += matrix.bellmanFordTest();
        timeList += list.bellmanFordTest();
    }
    timeMatrix /= 50;
    timeList /= 50;
    std::cout << "Bellman-Ford " << nodes1 << " | " << density << " || " << timeMatrix << " | " << timeList << std::endl;
}

Graph::Graph() = default;
