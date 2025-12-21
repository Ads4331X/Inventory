#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <cstring>
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>

class Item
{
protected:
    char _itemName[50];
    int _id;
    char _addedDateTime[20];

public:
    Item(std::string itemName = "", int id = 0);
    void addItem();
    void editItem(int editId);
    void deleteItem(int deleteId);
    void viewInv();
    void delItem();
    virtual void display() = 0;
};

#endif
