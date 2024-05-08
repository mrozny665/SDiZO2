#include <iostream>
#include "list.h"
#include "../structures/heap.h"
#include "util/test.h"

AdjList::AdjList() = default;

AdjList::AdjList(int nodes, int edges, bool directed) {
    nodeCount = nodes;
    edgeCount = edges;
    AdjList::directed = directed;
    list = new List[nodeCount];
}

AdjList::AdjList(int nodes, int edges, bool directed, int startNode) {
    nodeCount = nodes;
    edgeCount = edges;
    AdjList::directed = directed;
    AdjList::startNode = startNode;
    list = new List[nodeCount];
}

AdjList::AdjList(int nodes, int edges, bool directed, int startNode, int endNode) {
    nodeCount = nodes;
    edgeCount = edges;
    AdjList::directed = directed;
    AdjList::startNode = startNode;
    AdjList::endNode = endNode;
    list = new List[nodeCount];
}

AdjList::~AdjList() = default;

void AdjList::generateUndirected(Edge *edges) const {
    for (int i = 0; i < nodeCount; i++){
        list[i] = *new List();
    }
    for (int i = 0; i < edgeCount; i++){
        int node1 = edges[i].node1;
        int node2 = edges[i].node2;
        int weight = edges[i].weight;
        list[node1].addElement2(node2, weight);
        list[node2].addElement2(node1, weight);
    }
}

void AdjList::generateDirected(Edge *edges) const {
    for (int i = 0; i < nodeCount; i++){
        list[i] = *new List();
    }
    for (int i = 0; i < edgeCount; i++) {
        int node1 = edges[i].node1;
        int node2 = edges[i].node2;
        int weight = edges[i].weight;
        list[node1].addElement2(node2, weight);
    }
}

void AdjList::display() const {
    for (int i = 0; i < nodeCount; i++){
        std::cout << i << " -> ";
        list[i].display();
    }
}

void AdjList::primAlgorithm(int startingNode) {
    int key[nodeCount]; //key - obecna długość drogi do danego wierzchołka
    int p[nodeCount]; //p - rodzic danego wierzchołka (wierzchołek do którego jest połączony)
    bool inTree[nodeCount]; //sprawdzenie czy wierzchołek został już przyłączony do drzewa
    for (int i = 0; i < nodeCount; i++){ //inicjalizacja algorytmu
        key[i] = 10000;
        p[i] = -1;
        inTree[i] = false;
    }
    key[startingNode] = 0; //droga do wierzchołka startowego jest równa 0
    for (int i = 0; i < nodeCount; i++){
        int min = 10000;
        int minIndex;
        for (int k = 0; k < nodeCount; k++){ //poszukiwanie wierzchołka o najniższej drodze i jeszcze nie dodanego
            if (key[k] < min && !inTree[k]){ //zapisanie wierzchołka o mniejszej drodze
                min = key[k];
                minIndex = k;
            }
        }
        inTree[minIndex] = true; //dołączenie wierzchołka do drzewa
        ListElement *element = list[minIndex].getHeadPointer();
        for (int j = 0; j < list[minIndex].getListSize(); j++){ //przeszukiwanie wierzchołków sąsiadujących z obecnie dołączanym
            if (!inTree[element->node] && element->weight < key[element->node]){ //jeżeli sąsiad nie jest w drzewie, a jego obecna droga jest dłuższa niż możliwa nowa
                p[element->node] = minIndex; //zapisanie tej nowej drogi
                key[element->node] = element->weight;
            }
            element = element->next;
        }
    }
    int weight = 0;
    int currWeight;
    for (int i = 0; i < nodeCount; i++){
        if (p[i] != -1) {
            ListElement *element = list[i].getHeadPointer();
            for (int j = 0; j < list[i].getListSize(); j++) {
                if (element->node == p[i]) {
                    currWeight = element->weight;
                }
                element = element->next;
            }
            std::cout << i << "|" << p[i] << "|" << currWeight << std::endl;
            weight += currWeight;
        }
    }
    std::cout << weight << std::endl;
}

void AdjList::kruskalAlgorithm() {
    Heap heap = *new Heap();
    for (int i = 0; i < nodeCount; i++){
        ListElement *element = list[i].getHeadPointer();
        for (int j = 0; j < list[i].getListSize(); j++){
            if (!heap.searchHeap(i, element->node, element->weight)){
                heap.addElement(i, element->node, element->weight); //dodawanie kolejnych krawędzi do kopca
            }
            element = element->next;
        }
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

int AdjList::FindSet(int x, int *parent) {
    if (parent[x] != x) parent[x] = FindSet(parent[x], parent); //maksymalne możliwe skracanie drzewa - jako rodzic elementu
                                                                   //jest ustawiany korzeń drzewa, w którym się on znajduje
    return parent[x];
}

void AdjList::Union(int x, int y, int *parent, int *rank) {
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

void AdjList::dijkstraAlgorithm() {
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
        ListElement *adj = list[node].getHeadPointer();
        for (int i = 0; i < list[node].getListSize(); i++){ //przeglad listy sasiadow rozpatrywanego wierzcholka
            if (d[adj->node] > d[node] + adj->weight){ //relaksacja
                d[adj->node] = d[node] + adj->weight;
                heap.replace(adj->node, 0, d[adj->node]);
                p[adj->node] = node;
            }
            adj = adj->next;
        }
        heap.heapify(); //przywrocenie wlasnosci kopca
    }
    std::cout << "Lista:" << std::endl;
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

void AdjList::bellmanFordAlgorithm() {
    Edge edges[edgeCount]; //tablica ze wszystkimi krawedziami grafu
    int d[nodeCount]; //odleglosc danego wierzcholka od wierzcholka startowego
    int p[nodeCount]; //poprzedni wierzcholek w sciezce
    int k = 0; //licznik krawedzi
    for (int i = 0; i < nodeCount; i++){ //inicjalizacja tablic d i p
        d[i] = 10000;
        p[i] = -1;
        ListElement *adj = list[i].getHeadPointer();
        for (int j = 0; j < list[i].getListSize(); j++){ //tworzenie tablicy krawedzi
            Edge newEdge{i, adj->node, adj->weight};
            edges[k++] = newEdge;
            adj = adj->next;
        }
    }
    d[startNode] = 0; //wierzcholek startowy ma nadana odleglosc 0
    for (int i = 0; i < nodeCount-1; i++){ //powtarzanie V-1 liczbe razy
        for (int j = 0; j < edgeCount; j++){ //sprawdzanie kazdej krawedzi w w tablicy
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
    std::cout << "Lista:" << std::endl;
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

double AdjList::primTest(int startingNode) {
    double time = 0;
    Test::StartCounter();
    int key[nodeCount]; //key - obecna długość drogi do danego wierzchołka
    int p[nodeCount]; //p - rodzic danego wierzchołka (wierzchołek do którego jest połączony)
    bool inTree[nodeCount]; //sprawdzenie czy wierzchołek został już przyłączony do drzewa
    for (int i = 0; i < nodeCount; i++){ //inicjalizacja algorytmu
        key[i] = 10000;
        p[i] = -1;
        inTree[i] = false;
    }
    key[startingNode] = 0; //droga do wierzchołka startowego jest równa 0
    for (int i = 0; i < nodeCount; i++){
        int min = 10000;
        int minIndex;
        for (int k = 0; k < nodeCount; k++){ //poszukiwanie wierzchołka o najniższej drodze i jeszcze nie dodanego
            if (key[k] < min && !inTree[k]){ //zapisanie wierzchołka o mniejszej drodze
                min = key[k];
                minIndex = k;
            }
        }
        inTree[minIndex] = true; //dołączenie wierzchołka do drzewa
        ListElement *element = list[minIndex].getHeadPointer();
        for (int j = 0; j < list[minIndex].getListSize(); j++){ //przeszukiwanie wierzchołków sąsiadujących z obecnie dołączanym
            if (!inTree[element->node] && element->weight < key[element->node]){ //jeżeli sąsiad nie jest w drzewie, a jego obecna droga jest dłuższa niż możliwa nowa
                p[element->node] = minIndex; //zapisanie tej nowej drogi
                key[element->node] = element->weight;
            }
            element = element->next;
        }
    }
    time += Test::GetCounter();
    return time;
}

double AdjList::kruskalTest() {
    double time = 0;
    Test::StartCounter();
    Heap heap = *new Heap();
    for (int i = 0; i < nodeCount; i++){
        ListElement *element = list[i].getHeadPointer();
        for (int j = 0; j < list[i].getListSize(); j++){
            if (!heap.searchHeap(i, element->node, element->weight)){
                heap.addElement(i, element->node, element->weight); //dodawanie kolejnych krawędzi do kopca
            }
            element = element->next;
        }
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
    time += Test::GetCounter();
    return time;
}

double AdjList::dijkstraTest() {
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
        ListElement *adj = list[node].getHeadPointer();
        for (int i = 0; i < list[node].getListSize(); i++){
            if (d[adj->node] > d[node] + adj->weight){
                d[adj->node] = d[node] + adj->weight;
                heap.replace(adj->node, 0, d[adj->node]);
                p[adj->node] = node;
            }
            adj = adj->next;
        }
        heap.heapify();
    }
    time += Test::GetCounter();
    return time;
}

double AdjList::bellmanFordTest() {
    double time = 0;
    Test::StartCounter();
    Edge edges[edgeCount];
    int d[nodeCount];
    int p[nodeCount];
    int k = 0;
    for (int i = 0; i < nodeCount; i++){
        d[i] = 10000;
        p[i] = -1;
        ListElement *adj = list[i].getHeadPointer();
        for (int j = 0; j < list[i].getListSize(); j++){
            Edge newEdge{i, adj->node, adj->weight};
            edges[k++] = newEdge;
            adj = adj->next;
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

