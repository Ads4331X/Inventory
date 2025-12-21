#include "product.h"
#include <iostream>
#include "Item.h"
#include "config.h"
#include "TimeUtils.h"

Product::Product(std::string itemName, int id, double price, int quantity, std::string description)
    : Item(itemName, id)
{
    _price = price;
    _quantity = quantity;
    strncpy(_description, description.c_str(), sizeof(_description));
    _description[sizeof(_description) - 1] = '\0';
}

void Product::addItem()
{
    std::cout << "---------------------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Enter item details:" << std::endl;
    std::cout << "Enter item name: ";
    std::cin.getline(_itemName, 50);
    std::cout << "Enter item ID: ";
    std::cin >> _id;
    std::cout << "Enter item price: ";
    std::cin >> _price;
    std::cout << "Enter item quantity: ";
    std::cin >> _quantity;
    std::cout << "Enter item description: ";
    std::cin.ignore();
    std::cin.getline(_description, 100);
    std::cout << "---------------------------------------------------------------------------------------------------------------------" << std::endl;
    strncpy(_addedDateTime, getCurrentDateTime().c_str(), sizeof(_addedDateTime));
    _addedDateTime[sizeof(_addedDateTime) - 1] = '\0';

    invFile.open(inventoryFilename, std::ios::app | std::ios::binary);
    if (!invFile)
    {
        std::cerr << "Error opening file for adding item!" << std::endl;
        return;
    }
    invFile.write((char *)this, sizeof(Product));
    invFile.close();

    std::cout << "Product added successfully!" << std::endl;
}