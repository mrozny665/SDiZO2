#ifndef SDIZO1_HEAP_H
#define SDIZO1_HEAP_H

#include<string>
#include "../graphs/util/edge.h"


class Heap {

    int heapSize; //obecna liczba elementow kopca
    Edge heapArray[20000]{}; //tablica przechowujaca elementy kopca - wartosc przyjeta jest nadmiarowa
public:
    Heap(); //konstruktor
    ~Heap(); //destruktor

    void addElement(int value1, int value2, int weight); //funkcja dodajaca element do kopca
    bool removeElement(int index); //funkcja usuwajaca element z kopca
    bool searchHeap(int vertex1, int vertex2, int weight); // funkcja szukajaca wartosci w kopcu
    bool search2(int vertex);
    void loadFromFile(std::string fileName); //funkcja generujaca kopiec z pliku txt
    void generateHeap(int size); //funkcja generujaca losowy kopiec
    void display(); //funkcja wyswietlajaca kopiec
    void fixFromTop(int index); //funkcja przywracajaca wlasnosc kopca od gory
    void fixFromBottom(int index); //funkcja przywracajaca wlasnosc kopca od dolu
    int getHeapSize() const; //funkcja zwracajaca liczbe elementow kopca
    void replace(int node, int val1, int val2);

    void removeRoot(); //funkcja usuwajaca korzen kopca
    Edge getRoot();
    int getRoot2();

    void heapify();
};


#endif //SDIZO1_HEAP_H
