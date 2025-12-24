#include "Item.h"
#include "config.h"
#include "TimeUtils.h"

/*Constructor: Initializes Item with name and ID, sets current date/time
 Parameters: itemName - name of the item, id - unique identifier */
Item::Item(std::string itemName = "", int id = 0)
{
    strncpy(_itemName, itemName.c_str(), sizeof(_itemName)); // copy the item name
    _itemName[sizeof(_itemName) - 1] = '\0';                 // ensure null-terminated
    _id = id;                                                // set item id

    // Set current date and time when item is created
    strncpy(_addedDateTime, getCurrentDateTime().c_str(), sizeof(_addedDateTime));
    _addedDateTime[sizeof(_addedDateTime) - 1] = '\0';
}

/* function to add item to inventory file
 Note: This is overridden in derived classes
*/
void Item::addItem()
{

    // update the date and time to current time
    strncpy(_addedDateTime, getCurrentDateTime().c_str(), sizeof(_addedDateTime));
    _addedDateTime[sizeof(_addedDateTime) - 1] = '\0';

    invFile.open(inventoryFilename, std::ios::app | std::ios::binary); // opens inventory file in append in binary mode
    if (!invFile)                                                      // checks if inventory file exists
    {
        std::cerr << "Error opening file for adding item!" << std::endl; // display error message
        return;
    }
    invFile.write((char *)&*this, sizeof(*this)); // writes the item object to inventory file
    invFile.close();                              // close the file
}

/* function to edit item which takes an id(editId) from the user to edit that data
 */
void Item::editItem(int editId)
{
    invFile.open(inventoryFilename, std::ios::in | std::ios::binary);       // open inventory file in reading in binary mode
    tempFile.open(tempInventoryFilename, std::ios::out | std::ios::binary); // open temp file in writing in binary mode
    if (!invFile || !tempFile)                                              // checks if inventory or temp file exists or not
    {
        std::cerr << "Error opening files for editing item!" << std::endl; // displays error
        return;
    }
    while (invFile.read((char *)&*this, sizeof(*this))) // read the inventory file
    {
        if (_id == editId) // checks if the id is equal to id (editId) given by user
        {
            std::string newName; // initialize new name for the item
            int newId;           // initialize new id for the item
            std::cout << "Enter new item name: ";
            std::getline(std::cin >> std::ws, newName); // gets the name of the item to be changed
            std::cout << "Enter new item ID: ";
            std::cin >> newId;                                      // gets the  id of the product to be changed
            strncpy(_itemName, newName.c_str(), sizeof(_itemName)); // copies the changed name to the item name
            _itemName[sizeof(_itemName) - 1] = '\0';                // ensure null-terminated
            _id = newId;                                            // sets the new id
        }
        tempFile.write((char *)&*this, sizeof(*this)); // writes the item object in temp file
    }
    // closing files
    invFile.close();
    tempFile.close();

    remove(inventoryFilename.c_str());                                // remove old file
    rename(tempInventoryFilename.c_str(), inventoryFilename.c_str()); // rename files
}

/* function to delete an item from the inventory which takes deleteId as parameter and checks if the id is equal to deleteId*/
void Item::deleteItem(int deleteId)

{
    invFile.open(inventoryFilename, std::ios::in | std::ios::binary);       // open inventory file in reading in binary mode
    tempFile.open(tempInventoryFilename, std::ios::out | std::ios::binary); // open temp file in writing in binary mode
    historyFile.open(historyFilename, std::ios::app | std::ios::binary);    // open history file in append in binary mode
    if (!invFile || !tempFile || !historyFile)                              // check if those three file exists or not
    {
        std::cerr << "Error opening files for deleting item!" << std::endl; // display error
        return;
    }
    while (invFile.read((char *)&*this, sizeof(*this))) // read inventory file
    {
        if (_id == deleteId) // check if the id is equal to deleteId
        {

            // Log deleted item to history
            historyFile.write((char *)&*this, sizeof(*this));
            std::cout << "Item deleted successfully!" << std::endl;
            continue; // Skip writing to temp file
        }
        tempFile.write((char *)&*this, sizeof(*this)); // writes the item object in temp file
    }

    // closing files
    invFile.close();
    tempFile.close();
    historyFile.close();

    remove(inventoryFilename.c_str());                                // remove old file
    rename(tempInventoryFilename.c_str(), inventoryFilename.c_str()); // rename files
}

/* function to display the data of item that user has just entered*/
void Item::display()
{
    // display the data of item that user has just entered
    std::cout << "Item ID: " << _id << ", Item Name: " << _itemName << "Added Date: " << _addedDateTime << std::endl;
}

/* function to view all data of inventory */
void Item::viewInv()
{
    invFile.open(inventoryFilename, std::ios::in | std::ios::binary); // Open inventory file in reading in binary mode
    if (!invFile)                                                     // checks if inventory file exists
    {
        std::cerr << "Error opening file for viewing inventory!" << std::endl; // displays error
        return;
    }
    while (invFile.read((char *)&*this, sizeof(*this))) // reads inventory file
    {
        display(); // to display the inventory
    }
    invFile.close(); // close the inventory file
}

/* function to view the data of the deleted items
it displays the data stored in history file which stores all the deleted item information*/
void Item::delItem()
{
    historyFile.open(historyFilename, std::ios::in | std::ios::binary); // opens history file in reading in binary mode
    if (!historyFile)                                                   // checks if history file exists
    {
        std::cerr << "Error opening file for viewing history!" << std::endl; // displays error
        return;
    }
    while (historyFile.read((char *)&*this, sizeof(*this))) // reads history file
    {
        display(); // to display the history
    }
    historyFile.close(); // close the history file
}