#include <iostream>
#include "graphs/graph.h"

using namespace std;

void displayMenu(const string& info){
    cout << endl;
    cout << info << endl;
    cout << "0. Wyjscie" << endl;
    cout << "1. Wczytaj z pliku" << endl;
    cout << "2. Wygeneruj losowo" << endl;
    cout << "3. Wyswietl graf" << endl;
}

void menu_mst(){
    char opt;
    Graph graph;
    int vertices;
    int density;
    int startNode;
    string fileName;

    int nodeCounts[7] = {20, 50, 80, 100, 130, 160, 200};
    int densities[3] = {20, 50, 99};

    do {
        displayMenu("--- MINIMALNE DRZEWO ROZPINAJACE ---");
        cout << "4. Algorytm Prima" << endl;
        cout << "5. Algorytm Kruskala" << endl;
        cout << "Podaj opcje:";
        cin >> opt;

        switch (opt){
            case '1':
                cout << " Podaj nazwe pliku (z rozszerzeniem): ";
                cin >> fileName;
                graph.loadFromFile(fileName, 0);
                graph.display();
                break;
            case '2':
                cout << " Podaj liczbe wierzcholkow: ";
                cin >> vertices;
                cout << " Podaj gestosc grafu: ";
                cin >> density;
                graph.generateUndirected(vertices, density);
                graph.display();
                break;
            case '3':
                graph.display();
                break;
            case '4':
                cout << " Podaj wierzcholek poczatkowy:";
                cin >> startNode;
                graph.primAlgorithm(startNode);
                break;
            case '5':
                graph.kruskalAlgorithm();
                break;

            case 'q':
                for (int nodeCount : nodeCounts){
                    for (int density1 : densities){
                        graph.primTest(nodeCount,density1);
                        graph.kruskalTest(nodeCount,density1);
                    }
                }
                break;

            case '0':
                break;
            default:
                cout << "Blad, wybierz ponownie" << endl;
                break;
        }
    } while (opt != '0');
}

void menu_short_path(){
    char opt;
    Graph graph;
    int vertices;
    int density;
    string fileName;

    int nodeCounts[7] = {20, 50, 80, 100, 130, 160, 200};
    int densities[3] = {20, 50, 99};

    do {
        displayMenu("--- NAJKROTSZA SCIEZKA W GRAFIE ---");
        cout << "4. Algorytm Dijkstry" << endl;
        cout << "5. Algorytm Bellmana-Forda" << endl;
        cout << "Podaj opcje:";
        cin >> opt;

        switch (opt){
            case '1':
                cout << " Podaj nazwe pliku (z rozszerzeniem): ";
                cin >> fileName;
                graph.loadFromFile(fileName, 1);
                graph.display();
                break;
            case '2':
                cout << " Podaj liczbe wierzcholkow: ";
                cin >> vertices;
                cout << " Podaj gestosc grafu: ";
                cin >> density;
                graph.generateDirected(vertices, density);
                graph.display();
                break;
            case '3':
                graph.display();
                break;
            case '4':
                graph.dijkstraAlgorithm();
                break;
            case '5':
                graph.bellmanFordAlgorithm();
                break;

            case 'q':
                for (int nodeCount : nodeCounts){
                    for (int density1 : densities){
                        graph.dijkstraTest(nodeCount,density1);
                        graph.bellmanFordTest(nodeCount,density1);
                    }
                }
                break;

            case '0':
                break;
            default:
                cout << "Blad, wybierz ponownie" << endl;
                break;
        }
    } while (opt != '0');
}

int main() {
    char option;
    do {
        cout << endl;
        cout << "==== MENU GLOWNE ===" << endl;
        cout << "0. Wyjscie" << endl;
        cout << "1. Minimalne drzewo rozpinajace" << endl;
        cout << "2. Najkrotsza sciezka" << endl;
        cout << "Podaj opcje:";
        cin >> option;
        cout << endl;

        switch (option) {
            case '1':
                menu_mst();
                break;

            case '2':
                menu_short_path();
                break;

            case '0':
                break;

            default:
                cout << "Blad, wybierz ponownie";
                break;
        }
    } while (option != '0');
    return 0;
}
