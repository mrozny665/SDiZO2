#include <iostream>
#include <iomanip>
#include "matrix.h"
#include "../structures/heap.h"
#include "util/test.h"

Matrix::Matrix() = default;

Matrix::Matrix(int nodes, int edges, bool directed) {
    nodeCount = nodes;
    edgeCount = edges;
    Matrix::directed = directed;
    matrix = new int*[nodeCount];
}

Matrix::Matrix(int nodes, int edges, bool directed, int startNode) {
    nodeCount = nodes;
    edgeCount = edges;
    Matrix::directed = directed;
    Matrix::startNode = startNode;
    matrix = new int*[nodeCount];
}

Matrix::Matrix(int nodes, int edges, bool directed, int startNode, int endNode) {
    nodeCount = nodes;
    edgeCount = edges;
    Matrix::directed = directed;
    Matrix::startNode = startNode;
    Matrix::endNode = endNode;
    matrix = new int*[nodeCount];
}

Matrix::~Matrix() = default;

void Matrix::generateUndirected(Edge *edges) const {
    for (int i = 0; i < nodeCount; i++){
        matrix[i] = new int[nodeCount];
        for (int j = 0; j < nodeCount; j++){
            matrix[i][j] = 0;
        }
    }
    for (int i = 0; i < edgeCount; i++){
        int a = edges[i].node1;
        int b = edges[i].node2;
        matrix[a][b] = edges[i].weight;
        matrix[b][a] = edges[i].weight;
    }
}

void Matrix::generateDirected(Edge *edges) const{
    for (int i = 0; i < nodeCount; i++){
        matrix[i] = new int[nodeCount];
        for (int j = 0; j < nodeCount; j++){
            matrix[i][j] = 0;
        }
    }
    for (int i = 0; i < edgeCount; i++){
        int a = edges[i].node1;
        int b = edges[i].node2;
        matrix[a][b] = edges[i].weight;
    }
}

void Matrix::primAlgorithm(int startingNode) {
    int key[nodeCount]; //key - obecna długość drogi do danego wierzchołka
    int p[nodeCount]; //p - rodzic danego wierzchołka (wierzchołek do którego jest połączony)
    bool inTree[nodeCount]; //sprawdzenie czy wierzchołek został już przyłączony do drzewa
    for (int i = 0; i < nodeCount; i++){ //inicjalizacja algorytmu
        key[i] = INT_MAX;
        p[i] = -1;
        inTree[i] = false;
    }
    key[startingNode] = 0; //droga do wierzchołka startowego jest równa 0
    for (int i = 0; i < nodeCount; i++){
        int min = INT_MAX;
        int minIndex;
        for (int k = 0; k < nodeCount; k++){ //poszukiwanie wierzchołka o najniższej drodze i jeszcze nie dodanego
            if (key[k] < min && !inTree[k]){ //zapisanie wierzchołka o mniejszej drodze
                min = key[k];
                minIndex = k;
            }
        }
        inTree[minIndex] = true; //dołączenie wierzchołka do drzewa
        for (int j = 0; j < nodeCount; j++){ //przeszukanie wierzchołków sąsiadujących - zapewnia to warunek matrix[][] != 0
            if (matrix[minIndex][j] != 0 && !inTree[j] && matrix[minIndex][j] < key[j]){ //jeżeli wierzchołek jest sąsiadem, nie jest w drzewie,
                                                                                         // a jego obecna droga jest dłuższa niż możliwa nowa
                p[j] = minIndex; //zapisanie tej nowej drogi
                key[j] = matrix[minIndex][j];
            }
        }
    }
    int weight = 0;
    for (int i = 0; i < nodeCount; i++){
        if (p[i] != -1) {
            std::cout << i << "|" << p[i] << "|" << matrix[i][p[i]] << std::endl;
            weight += matrix[i][p[i]];
        }
    }
    std::cout << weight << std::endl;
}

void Matrix::kruskalAlgorithm() {
    Heap heap = *new Heap();
    for (int i = 0; i < nodeCount; i++){
        for (int j = i; j < nodeCount; j++){
            if (matrix[i][j] != 0){
                heap.addElement(i,j,matrix[i][j]); //dodawanie kolejnych krawędzi do kopca
            }                                                           //w każdej kolejnej linii macierzy rozpoczynamy od
        }                                                               //dalszej kolumny, żeby nie powtarzać krawędzi
    }
    for (int i = (heap.getHeapSize()-1)/2; i >= 0; i--){ //naprawa algorytmem Floyda
        heap.fixFromTop(i);
    }
    int parent[nodeCount];
    int rank[nodeCount];
    Edge MST[nodeCount-1];
    for (int i = 0; i < nodeCount; i++){ //tworzenie zbiorów rozłącznych - każdy wierzchołek jest swoim rodzicem i ma wysokość drzewa = 0
        parent[i] = i;
        rank[i] = 0;
    }
    int addedEdges = 0;
    while (heap.getHeapSize() > 0){
        Edge edge = heap.getRoot();
        if (FindSet(edge.node1, parent) != FindSet(edge.node2, parent)){ //sprawdzanie czy wierzchołki należą do tego samego zbioru
            MST[addedEdges++] = edge;
            Union(edge.node1, edge.node2, parent, rank); //łączenie zbiorów
        }
        heap.removeRoot(); //usuwanie krawędzi z kolejki
    }
    int sum = 0;
    for (Edge i:MST){
        sum += i.weight;
        std::cout << i.node1 << "|" << i.node2 << "|" << i.weight << std::endl;
    }
    std::cout << sum << std::endl;
}

void Matrix::dijkstraAlgorithm() {
    int d[nodeCount]; //odleglosc danego wierzcholka od wierzcholka poczatkowego
    int p[nodeCount]; //poprzedni wiercholek w sciezce
    Heap heap = *new Heap();
    for (int i = 0; i < nodeCount; i++){ //inicjalizacja tablic d i p oraz kopca
        d[i] = 10000;
        p[i] = -1;
        heap.addElement(i, 0, d[i]);
    }
    d[startNode] = 0; //wierzcholek startowy ma nadana odleglosc 0
    heap.replace(startNode, 0, 0);
    heap.heapify();
    while (heap.getHeapSize() != 0){ //sprawdzenie czy na kopcu jeszcze znajduja sie wierzcholki
        int node = heap.getRoot2();
        heap.removeRoot(); //pobranie wartosci i usuniecie korzenia - wierzcholka o najmniejszym d
        for (int i = 0; i < nodeCount; i++){ //przegladanie calego wiersza macierzy
            if (matrix[node][i] != 0){ //sprawdzenie obecnosci sciezki - 0 = nie ma sciezki
                if (d[i] > d[node] + matrix[node][i]){ //relaksacja
                    d[i] = d[node] + matrix[node][i];
                    heap.replace(i, 0, d[i]);
                    p[i] = node;
                }
            }
        }
        heap.heapify(); //przywrocenie wlasnosci stosu
    }
    std::cout << "Macierz:" << std::endl;
    for (int i = 0; i < nodeCount; i++){
        std::cout << i << ": ";
        int prev = p[i];
        while (prev != -1){
            std::cout << " <- " << prev;
            prev = p[prev];
        }
        std::cout << " | " << d[i] << std::endl;
    }
}

void Matrix::bellmanFordAlgorithm() {
    Edge edges[edgeCount]; //tablica ze wszystkimi krawedziami grafu
    int d[nodeCount]; //odleglosc danego wierzcholka od wierzcholka startowego
    int p[nodeCount]; //poprzedni wierzcholek w sciezce
    int k = 0; //licznik krawedzi
    for (int i = 0; i < nodeCount; i++){ //inicjalizacja tablic d i p
        d[i] = 10000;
        p[i] = -1;
        for (int j = 0; j < nodeCount; j++){ //tworzenie tablicy krawedzi
            if (matrix[i][j] != 0){
                Edge newEdge{i, j, matrix[i][j]};
                edges[k++] = newEdge;
            }
        }
    }
    d[startNode] = 0; //wierzcholek startowy ma nadana odleglosc 0
    for (int i = 0; i < nodeCount-1; i++){ //powtarzanie V-1 liczbe razy
        for (int j = 0; j < edgeCount; j++){ //sprawdzanie kazdej krawedzi
            int node1 = edges[j].node1;
            int node2 = edges[j].node2;
            int weight = edges[j].weight;
            if (d[node2] > d[node1] + weight) { //relaksacja
                d[node2] = d[node1] + weight;
                p[node2] = node1;
            }
        }
    }
    for (int i = 0; i < edgeCount; i++){ //ostatnie sprawdzenie - zabezpieczenie przed cyklami ujemnymi
        int node1 = edges[i].node1;
        int node2 = edges[i].node2;
        int weight = edges[i].weight;
        if (d[node2] > d[node1] + weight) {
            std::cout << "Cykl ujemny" << std::endl;
            return;
        }
    }
    std::cout << "Macierz:" << std::endl;
    for (int i = 0; i < nodeCount; i++){
        std::cout << i << ": ";
        int prev = p[i];
        while (prev != -1){
            std::cout << " <- " << prev;
            prev = p[prev];
        }
        std::cout << " | " << d[i] << std::endl;
    }
}

void Matrix::display() const {
    std::cout << "   ";
    for (int i = 0; i < nodeCount; i++){
        std::cout << std::setw(2) << i << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < nodeCount; i++){
        std::cout << std::setw(2) << i << " ";
        for (int j = 0; j < nodeCount; j++){
            std::cout << std::setw(2) << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int Matrix::FindSet(int x, int *parent) {
    if (parent[x] != x) parent[x] = FindSet(parent[x], parent); //maksymalne możliwe skracanie drzewa - jako rodzic elementu
                                                                    // jest ustawiany korzeń drzewa, w którym się on znajduje
    return parent[x];
}

void Matrix::Union(int x, int y, int *parent, int *rank) {
    int a = FindSet(x, parent);
    int b = FindSet(y, parent);
    if (rank[a] < rank[b]){ //rodzicem zostaje to drzewo, które jest wyższe przed połączeniem
        parent[a] = b;
    } else {
        parent[b] = a;
    }
    if (rank[a] == rank[b]){ //jeżeli są równe, to element, który zostaje rodzicem ma powiększoną wysokość
        rank[a]++;
    }
}

double Matrix::primTest(int startingNode) {
    double time = 0;
    Test::StartCounter();
    int key[nodeCount];
    int p[nodeCount];
    bool inTree[nodeCount];
    for (int i = 0; i < nodeCount; i++){
        key[i] = INT_MAX;
        p[i] = -1;
        inTree[i] = false;
    }
    key[startingNode] = 0;
    for (int i = 0; i < nodeCount; i++){
        int min = INT_MAX;
        int minIndex;
        for (int k = 0; k < nodeCount; k++){
            if (key[k] < min && !inTree[k]){
                min = key[k];
                minIndex = k;
            }
        }
        inTree[minIndex] = true;
        for (int j = 0; j < nodeCount; j++){
            if (matrix[minIndex][j] != 0 && !inTree[j] && matrix[minIndex][j] < key[j]){
                p[j] = minIndex;
                key[j] = matrix[minIndex][j];
            }
        }
    }
    time = Test::GetCounter();
    return time;
}

double Matrix::kruskalTest() {
    double time = 0;
    Test::StartCounter();
    Heap heap = *new Heap();
    for (int i = 0; i < nodeCount; i++){
        for (int j = i; j < nodeCount; j++){
            if (matrix[i][j] != 0){
                heap.addElement(i,j,matrix[i][j]);
            }
        }
    }
    for (int i = (heap.getHeapSize()-1)/2; i >= 0; i--){
        heap.fixFromTop(i);
    }
    int parent[nodeCount];
    int rank[nodeCount];
    Edge MST[nodeCount-1];
    for (int i = 0; i < nodeCount; i++){
        parent[i] = i;
        rank[i] = 0;
    }
    int addedEdges = 0;
    while (heap.getHeapSize() > 0){
        Edge edge = heap.getRoot();
        if (FindSet(edge.node1, parent) != FindSet(edge.node2, parent)){
            MST[addedEdges++] = edge;
            Union(edge.node1, edge.node2, parent, rank);
        }
        heap.removeRoot();
    }
    time += Test::GetCounter();
    return time;
}

double Matrix::dijkstraTest() {
    double time = 0;
    Test::StartCounter();
    int d[nodeCount];
    int p[nodeCount];
    Heap heap = *new Heap();
    for (int i = 0; i < nodeCount; i++){
        d[i] = 10000;
        p[i] = -1;
        heap.addElement(i, 0, d[i]);
    }
    d[startNode] = 0;
    heap.replace(startNode, 0, 0);
    heap.heapify();
    while (heap.getHeapSize() != 0){
        int node = heap.getRoot2();
        heap.removeRoot();
        for (int i = 0; i < nodeCount; i++){
            if (matrix[node][i] != 0){
                if (d[i] > d[node] + matrix[node][i]){
                    d[i] = d[node] + matrix[node][i];
                    heap.replace(i, 0, d[i]);
                    p[i] = node;
                }
            }
        }
        heap.heapify();
    }
    time += Test::GetCounter();
    return time;
}

double Matrix::bellmanFordTest() {
    double time = 0;
    Test::StartCounter();
    Edge edges[edgeCount];
    int d[nodeCount];
    int p[nodeCount];
    int k = 0;
    for (int i = 0; i < nodeCount; i++){
        d[i] = 10000;
        p[i] = -1;
        for (int j = 0; j < nodeCount; j++){
            if (matrix[i][j] != 0){
                Edge newEdge{i, j, matrix[i][j]};
                edges[k++] = newEdge;
            }
        }
    }
    d[startNode] = 0;
    for (int i = 0; i < nodeCount-1; i++){
        for (int j = 0; j < edgeCount; j++){
            int node1 = edges[j].node1;
            int node2 = edges[j].node2;
            int weight = edges[j].weight;
            if (d[node2] > d[node1] + weight) {
                d[node2] = d[node1] + weight;
                p[node2] = node1;
            }
        }
    }
    for (int i = 0; i < edgeCount; i++){
        int node1 = edges[i].node1;
        int node2 = edges[i].node2;
        int weight = edges[i].weight;
        if (d[node2] > d[node1] + weight) {
            std::cout << "Cykl ujemny" << std::endl;
            time += Test::GetCounter();
            return time;
        }
    }
    time += Test::GetCounter();
    return time;
}
