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
    Item(std::string itemName = "", int id = 0); // Constructor
    virtual void addItem() = 0;
    virtual void editItem(int editId);
    virtual void deleteItem(int deleteId);
    virtual void display();
    virtual void viewDelItem();
    virtual void viewInv() = 0;
    virtual ~Item() {} // Virtual destructor
};

#endif
