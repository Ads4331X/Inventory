#include "product.h"
#include <iostream>
#include "Item.h"
#include "config.h"
#include "TimeUtils.h"
#include <iomanip>
#include <fstream>

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

void Product::editItem(int editId)
{
    invFile.open(inventoryFilename, std::ios::in | std::ios::binary);
    tempFile.open(tempInventoryFilename, std::ios::out | std::ios::binary);
    if (!invFile || !tempFile)
    {
        std::cerr << "Error opening files for editing item!" << std::endl;
        return;
    }
    while (invFile.read((char *)&*this, sizeof(Product)))
    {
        if (_id == editId)
        {
            std::string newName;
            int newId;
            std::cout << "Enter new item name: ";
            std::getline(std::cin >> std::ws, newName);
            std::cout << "Enter new item ID: ";
            std::cin >> newId;
            std::cout << "Enter new item price: ";
            std::cin >> _price;
            std::cout << "Enter new item quantity: ";
            std::cin >> _quantity;
            std::cout << "Enter new item description: ";
            std::cin.ignore();
            std::cin.getline(_description, 100);
            strncpy(_itemName, newName.c_str(), sizeof(_itemName));
            _itemName[sizeof(_itemName) - 1] = '\0'; // ensure null-terminated
            _id = newId;
        }
        tempFile.write((char *)&*this, sizeof(Product));
    }
    // closing files
    invFile.close();
    tempFile.close();

    remove(inventoryFilename.c_str());                                // remove old file
    rename(tempInventoryFilename.c_str(), inventoryFilename.c_str()); // rename files

    std::cout << "Product edited successfully!" << std::endl; // success message
}

void Product::deleteItem(int deleteId)
{
    invFile.open(inventoryFilename, std::ios::in | std::ios::binary);
    tempFile.open(tempInventoryFilename, std::ios::out | std::ios::binary);
    historyFile.open(historyFilename, std::ios::app | std::ios::binary);
    if (!invFile || !tempFile || !historyFile)
    {
        std::cerr << "Error opening files for deleting item!" << std::endl;
        return;
    }
    while (invFile.read((char *)&*this, sizeof(Product)))
    {
        if (_id == deleteId)
        {

            // Log deleted item to history
            historyFile.write((char *)&*this, sizeof(Product));
            std::cout << "Item deleted successfully!" << std::endl;
            continue; // Skip writing to temp file
        }
        tempFile.write((char *)&*this, sizeof(Product));
    }

    // closing files
    invFile.close();
    tempFile.close();
    historyFile.close();

    remove(inventoryFilename.c_str());                                // remove old file
    rename(tempInventoryFilename.c_str(), inventoryFilename.c_str()); // rename files

    std::cout << "Product deleted successfully!" << std::endl; // success message
}

void Product::viewInv()
{
    invFile.open(inventoryFilename, std::ios::in | std::ios::binary);
    if (!invFile)
    {
        std::cerr << "Error opening file for viewing inventory!" << std::endl;
        return;
    }

    std::cout << "--------------------------------------------------------------------------------------------\n";
    std::cout << "| " << std::left << std::setw(25) << "Item Name"
              << "| " << std::setw(10) << "Price"
              << "| " << std::setw(6) << "Qty"
              << "| " << std::setw(20) << "Added Date"
              << "|\n";
    std::cout << "--------------------------------------------------------------------------------------------\n";

    Product temp;
    while (invFile.read((char *)&temp, sizeof(Product)))
    {
        std::cout << "| " << std::left << std::setw(25) << temp._itemName
                  << "| " << std::setw(10) << std::fixed << std::setprecision(2) << temp._price
                  << "| " << std::setw(6) << temp._quantity
                  << "| " << std::setw(20) << temp._addedDateTime
                  << "|\n";
    }

    std::cout << "--------------------------------------------------------------------------------------------\n";
    invFile.close();
}

void Product::delItem()
{
    historyFile.open(historyFilename, std::ios::in | std::ios::binary);
    if (!historyFile)
    {
        std::cerr << "Error opening file for viewing history!" << std::endl;
        return;
    }
    std::cout << "--------------------------------------------------------------------------------------------\n";
    std::cout << "| " << std::left << std::setw(25) << "Item Name"
              << "| " << std::setw(10) << "Price"
              << "| " << std::setw(6) << "Qty"
              << "| " << std::setw(20) << "Added Date"
              << "|\n";
    std::cout << "--------------------------------------------------------------------------------------------\n";

    Product temp;
    while (historyFile.read((char *)&temp, sizeof(Product)))
    {
        std::cout << "| " << std::left << std::setw(25) << temp._itemName
                  << "| " << std::setw(10) << std::fixed << std::setprecision(2) << temp._price
                  << "| " << std::setw(6) << temp._quantity
                  << "| " << std::setw(20) << temp._addedDateTime
                  << "|\n";
    }

    std::cout << "--------------------------------------------------------------------------------------------\n";
    invFile.close();
}

void Product::display()
{
    std::cout << "Item ID: " << _id << ", Item Name: " << _itemName << ", Price: " << _price << ", Quantity: " << _quantity << ", Description: " << _description << ", Added Date: " << _addedDateTime << std::endl;
}