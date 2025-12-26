#ifndef PRODUCT_H
#define PRODUCT_H

#include "Item.h"

class Product : public Item
{
private:
    double _price;
    int _quantity;
    char _description[100];
    std::string getTruncatedDesc(int width) const;

public:
    Product(std::string itemName = "", int id = 0, double price = 0, int quantity = 0, std::string description = "");
    void addItem();
    int genetateId();
    void editItem(int editId);
    void deleteItem(int deleteId);
    void viewInv();
    void viewDelItem();
};

#endif