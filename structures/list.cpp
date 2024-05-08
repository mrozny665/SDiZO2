#include"list.h"
#include<iostream>
#include<fstream>
#include<cstdlib>

List::List() { //konstruktor listy
    List::headPointer = nullptr;
    List::tailPointer = nullptr;
    List::listSize = 0; //ustawia wskazniki na head oraz tail na NULL oraz wielkosc na 0
}

int List::getListSize() const {
    return listSize;
}

List::~List() { //destruktor listy
    clearList(); //wywoluje funkcje usuwajaca liste
}

void List::loadFromFile(std::string fileName) { //wczytywanie listy z pliku
    clearList(); //wywolanie funkcji usuwajacej poprzednia liste
    std::ifstream file(fileName);
    std::string line;
    int newListSize;
    file >> newListSize; //ladowanie pierwszej liczby z pliku jako liczby elementow
    for (int i = 0; i < newListSize; i++) {
        int value;
        file >> value;
        addElement(i, value, 0); //zapisywanie kolejnych elementow z pliku jako kolejne elementy listy
    }
}

bool List::addElement(int index, int value1, int value2) { //dodawanie elementow do tablicy
    if (index == 0) { //czesc odpowiadajaca za dodanie na poczatku
        auto *newElement = new ListElement; //tworzy wskaznik na nowy element
        (*newElement).prev = nullptr; //jako ze jest to element pierwszy to wskaznik na poprzedni jest NULLem
        (*newElement).next = headPointer; //wskaznik na nastepny element jest dotychczsowym wskaznikiem na head
        (*newElement).node = value1; //zapisanie wartosci elementu
        newElement->weight = value2;
        if (listSize == 0) tailPointer = newElement; // jezeli lista jest pusta to ustawia ten element rowniez jako tail
        else (*headPointer).prev = newElement; //jezeli nie jest to zapisuje w dotychczasowym headzie jako poprzedni
        headPointer = newElement; //zapisuje obecny element jako head
        listSize++; //powieksza rozmiar listy
        return true; //zglasza pomyslne wykonanie zadania
    } else if (index == listSize) { //czesc odpowiadajaca za dodawanie na koncu
        auto *newElement = new ListElement; //tworzy wskaznik na nowy element
        (*tailPointer).next = newElement; //zapisuje jako nastepny element dotychczasownego taila
        (*newElement).prev = tailPointer; // zapisuje dotyczasowy tail jako poprzedni element nowego
        (*newElement).next = nullptr; //jako ze nowy element jest ostatnim, ustawia wskaznik na nastepny na NULL
        (*newElement).node = value1; //zapisanie wartosci elementu
        newElement->weight = value2;
        tailPointer = newElement; //zapisanie nowego elemenu jako tail
        listSize++; //powiekszenie rozmiaru listy
        return true; // zgloszenie powodzenia zadania
    } else if (index < listSize) { //czesc odpowiadajaca za wstawianie w srodku listy
        auto *newElement = new ListElement; //tworzenie wskaznika na nowy element
        ListElement *prevElement = headPointer;
        for (int i = 0; i < index - 1; i++) { //przeszukiwanie tablicy do momentu znalezienia odpowiedniego indeksu
            prevElement = (*prevElement).next;
        }
        (*(*prevElement).next).prev = newElement; //nowy element zapisany jako poprzedni nastepnego
        (*newElement).next = (*prevElement).next; //nastepny zapisany jako nastepny nowego
        (*newElement).prev = prevElement; //poprzedni element zapisany jako poprzedni nowego
        (*prevElement).next = newElement; //nowy element zapisany jako nastepny poprzedniego
        (*newElement).node = value1; //zapisanie wartosci elementu
        newElement->weight = value2;
        listSize++; //zwiekszenie rozmiaru listy
        return true; //zgloszenie poprawnego wykonania zadania
    } else {
        return false; //jezeli indeks jest wiekszy niz wielkosc listy to zglaszane jest niewykonanie zadania
    }
}

void List::addElement2(int value1, int value2) {
    auto *newElement = new ListElement; //tworzy wskaznik na nowy element
    (*newElement).prev = tailPointer; // zapisuje dotyczasowy tail jako poprzedni element nowego
    (*newElement).next = nullptr; //jako ze nowy element jest ostatnim, ustawia wskaznik na nastepny na NULL
    (*newElement).node = value1; //zapisanie wartosci elementu
    newElement->weight = value2;
    if (listSize == 0) headPointer = newElement; // jezeli lista jest pusta to ustawia ten element rowniez jako tail
    else (*tailPointer).next = newElement;
    tailPointer = newElement; //zapisanie nowego elemenu jako tail
    listSize++; //powiekszenie rozmiaru listy
}

bool List::removeElement(int value) { //usuwanie elementow z listy
    ListElement *pointer = headPointer;
    bool found = false;
    do { //przeszukiwanie listy
        if ((*pointer).node == value) { //warunek stopu - pierwsza znaleziona wartosc
            found = true;
            break;
        }
        if ((*pointer).next == nullptr) break; //warunek stopu - brak nastepnego elementu
        pointer = (*pointer).next; //pobranie kolejnego elementu
    } while (true);
    if (found) { //jezeli wartosc zostala znaleziona - zmienna 'found' ustawiona na 'true'
        if ((*pointer).prev == nullptr) { //jezeli jest to element pierwszy - nie ma poprzedniego
            if ((*pointer).next == nullptr) { //jezeli nie ma tez nastepnego - jest jedynym elementem
                delete pointer; //zwalniana pamiec
                listSize--; //zmniejszana wielkosc listy
                headPointer = nullptr; //wskaznik na head -> NULL
                tailPointer = nullptr; //wskaznik na tail -> NULL
                return true; //poprawne wykonanie polecenia
            } else {
                (*(*pointer).next).prev = nullptr; //wskaznik na poprzedni element w nastepnym jest ustawiany na 'NULL'
                headPointer = (*pointer).next; //wskaznik na head <- nastepny element
                delete pointer; //zwolnienie pamieci
                listSize--; //zmniejszenie rozmiaru listy
                return true; //poprawne wykonanie polecenia
            }
        } else if ((*pointer).next == nullptr) { //jezeli jest to element ostatni - nie ma nastepnego
            (*(*pointer).prev).next = nullptr; //wskaznik na nastepny element w poprzednim jest ustawiany na 'NULL'
            tailPointer = (*pointer).prev; //wskaznik na tail <- poprzedni element
            delete pointer; //zwolnienie pamieci
            listSize--; //zmniejszenie rozmiaru listy
            return true; //poprawne wykonanie polecenia
        } else { //jezeli jest to element ze srodka listy
            (*(*pointer).prev).next = (*pointer).next; //nastepny -> nastepny poprzedniego
            (*(*pointer).next).prev = (*pointer).prev; //poprzedni -> poprzedni nastepnego
            delete pointer; //zwolnienie pamieci
            listSize--; //zmniejszenie rozmiaru listy
            return true; //poprawne wykonanie polecenia
        }
    } else return false; //jezeli nie znaleziono elementu - niewykonanie zadania
}

void List::removeElement2(int index) {
    if (index == 0){
        ListElement *nextHead = headPointer->next;
        nextHead->prev = nullptr;
        delete headPointer;
        headPointer = nextHead;
        listSize--;
    } else if (index == listSize-1){
        ListElement *nextTail = tailPointer->prev;
        nextTail->next = nullptr;
        delete tailPointer;
        tailPointer = nextTail;
        listSize--;
    } else {
        ListElement *thisElement = headPointer;
        for (int i = 0; i < index; i++){
            thisElement = thisElement->next;
        }
        thisElement->prev->next = thisElement->next; //nastepny -> nastepny poprzedniego
        thisElement->next->prev = thisElement->prev; //poprzedni -> poprzedni nastepnego
        delete thisElement; //zwolnienie pamieci
        listSize--; //zmniejszenie rozmiaru listy
    }
}

bool List::searchList(int value) { //przeszukiwanie listy
    ListElement *pointer = headPointer;
    do { //petla przeszukuje element po elemencie
        if ((*pointer).node == value || pointer->weight == value) { //warunek stopu - znaleziono element
            return true; //zwraca true
        }
        if ((*pointer).next == nullptr) return false; //warunek stopu - koniec listy - zwraca false
        pointer = (*pointer).next; //przejscie do kolejnego elementu
    } while (true);
}

void List::generateList(int number) { //generowanie losowej listy o wielkosci 'number'
    clearList(); //usuwanie poprzedniej listy
    srand((unsigned int)(size_t) time); //generowanie liczb pseudolosowych
    for (int i = 0; i < number; i++) { //dla kolejnych wartosci licznika i az do 'number'
        int value = rand();
        addElement(i, value, 0); //wywolanie funkcji tworzacej nowy element
    }

}

void List::display() { //wyswietlanie listy
    if (listSize == 0) return; //jezeli lista jest pusta - nie wyswietla nic
    ListElement element = *headPointer; //rozpoczecie od poczatku listy
    while (true) {
        std::cout << element.node << "|"; //wydrukowanie wartosci elementu
        std::cout << element.weight << " "; //wydrukowanie wartosci elementu
        if (element.next == nullptr) break; // warunek zatrzymania - brak nastepnego elementu
        element = *element.next; //przejscie do nastepnego elementu
    }
    std::cout << "\n"; //przejscie do nowej linii w konsoli
}

void List::clearList() { //usuwanie poprzedniej tablicy
    if (headPointer != nullptr && tailPointer != nullptr) { //warunek - czyszczenie tylko jezeli lista istnieje
        ListElement *pointer = headPointer;
        ListElement *nextPointer;
        while (true) {
            if ((*pointer).next == nullptr) { //jezeli element nie ma nastepnego
                delete pointer; //zwalnianie pamieci
                break; //wyjscie z petli
            } else { //jezeli jest nastepny element
                (*(*pointer).next).prev = nullptr; //poprzedni nastepnego -> NULL
                nextPointer = (*pointer).next; //wziecie nastepnego elementu
                delete pointer; //zwolnienie pamieci
                pointer = nextPointer;
            }
        }
        headPointer = nullptr; //ustawienie wskaznika na head na NULL
        tailPointer = nullptr; //analogicznie tail
        listSize = 0; //ustawienie rozmiaru listy na 0
    }
}

ListElement *List::getHeadPointer() const {
    return headPointer;
}

