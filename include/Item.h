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
    Item(std::string itemName = "", int id = 0) // Constructor{
    {
        strncpy(_itemName, itemName.c_str(), sizeof(_itemName));
        _itemName[sizeof(_itemName) - 1] = '\0';
        _id = id;
    }
    virtual void addItem() = 0;
    virtual void editItem(int editId) = 0;
    virtual void deleteItem(int deleteId) = 0;
    virtual void display() = 0;
    virtual void viewDelItem() = 0;
    virtual void viewInv() = 0;
    virtual ~Item() {} // Virtual destructor
    virtual void searchItem(std::string name) = 0;
    virtual bool checkFileEmpty(std::fstream &file) = 0;
    virtual int genetateId() = 0;
    virtual double genetateReport() = 0;
};

#endif
