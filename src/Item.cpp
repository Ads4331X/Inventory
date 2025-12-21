#include "Item.h"
#include "config.h"
#include "TimeUtils.h"

Item::Item(std::string itemName = "", int id = 0)
{
    strncpy(_itemName, itemName.c_str(), sizeof(_itemName));
    _itemName[sizeof(_itemName) - 1] = '\0'; // ensure null-terminated
    _id = id;
    strncpy(_addedDateTime, getCurrentDateTime().c_str(), sizeof(_addedDateTime));
    _addedDateTime[sizeof(_addedDateTime) - 1] = '\0';
}

void Item::addItem()
{
    strncpy(_addedDateTime, getCurrentDateTime().c_str(), sizeof(_addedDateTime));
    _addedDateTime[sizeof(_addedDateTime) - 1] = '\0';

    invFile.open(inventoryFilename, std::ios::app | std::ios::binary);
    if (!invFile)
    {
        std::cerr << "Error opening file for adding item!" << std::endl;
        return;
    }
    invFile.write((char *)&*this, sizeof(*this));
    invFile.close();
}

void Item::editItem(int editId)
{
    invFile.open(inventoryFilename, std::ios::in | std::ios::binary);
    tempFile.open(tempInventoryFilename, std::ios::out | std::ios::binary);
    if (!invFile || !tempFile)
    {
        std::cerr << "Error opening files for editing item!" << std::endl;
        return;
    }
    while (invFile.read((char *)&*this, sizeof(*this)))
    {
        if (_id == editId)
        {
            std::string newName;
            int newId;
            std::cout << "Enter new item name: ";
            std::getline(std::cin >> std::ws, newName);
            std::cout << "Enter new item ID: ";
            std::cin >> newId;
            strncpy(_itemName, newName.c_str(), sizeof(_itemName));
            _itemName[sizeof(_itemName) - 1] = '\0'; // ensure null-terminated
            _id = newId;
        }
        tempFile.write((char *)&*this, sizeof(*this));
    }
    // closing files
    invFile.close();
    tempFile.close();

    remove(inventoryFilename.c_str());                                // remove old file
    rename(tempInventoryFilename.c_str(), inventoryFilename.c_str()); // rename files
}

void Item::deleteItem(int deleteId)

{
    invFile.open(inventoryFilename, std::ios::in | std::ios::binary);
    tempFile.open(tempInventoryFilename, std::ios::out | std::ios::binary);
    historyFile.open(historyFilename, std::ios::app | std::ios::binary);
    if (!invFile || !tempFile || !historyFile)
    {
        std::cerr << "Error opening files for deleting item!" << std::endl;
        return;
    }
    while (invFile.read((char *)&*this, sizeof(*this)))
    {
        if (_id == deleteId)
        {

            // Log deleted item to history
            historyFile.write((char *)&*this, sizeof(*this));
            std::cout << "Item deleted successfully!" << std::endl;
            continue; // Skip writing to temp file
        }
        tempFile.write((char *)&*this, sizeof(*this));
    }

    // closing files
    invFile.close();
    tempFile.close();
    historyFile.close();

    remove(inventoryFilename.c_str());                                // remove old file
    rename(tempInventoryFilename.c_str(), inventoryFilename.c_str()); // rename files
}

void Item::display()
{
    std::cout << "Item ID: " << _id << ", Item Name: " << _itemName << "Added Date: " << _addedDateTime << std::endl;
}

void Item::viewInv()
{
    invFile.open(inventoryFilename, std::ios::in | std::ios::binary);
    if (!invFile)
    {
        std::cerr << "Error opening file for viewing inventory!" << std::endl;
        return;
    }
    while (invFile.read((char *)&*this, sizeof(*this)))
    {
        display();
    }
    invFile.close();
}
