#ifndef LIST_H
#define LIST_H

#include<string>
#include "listElement.h"

class List {
	ListElement *headPointer;
public:
    ListElement *getHeadPointer() const;

private:
    //wskaznik na pierwszy element
	ListElement *tailPointer; //wskaznik na ostatni element
	int listSize; //obecna wielkosc listy

public:
	List(); //konstruktor
	~List(); //destruktor

	void loadFromFile(std::string fileName); //funkcja ladujaca liste z pliku
	bool addElement(int index, int value1, int value2); //funkcja dodajaca element do listy
    void addElement2(int value1, int value2);
	bool removeElement(int value); //funkcja usuwajaca element z listy
    void removeElement2(int index);
	bool searchList(int value); //funkcja szukajaca wartosci na liscie
	void generateList(int number); //funkcja tworzaca losowa liste
	void display(); //funkcja wyswietlajaca liste
	void clearList();

    int getListSize() const;
    //funkcja usuwajaca istnejaca liste
};
#endif /* LIST_H */