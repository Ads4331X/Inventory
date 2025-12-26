#include "product.h"
#include "../include/sleep.h"
#include <iostream>
#include "Item.h"
#include "config.h"
#include "TimeUtils.h"
#include <iomanip>
#include <fstream>

/*Constructor: Initializes Product with name, ID, price, quantity, and description
 Parameters: itemName - name of the item, id - unique identifier, price - price of the product,
 quantity - available quantity, description - description of the product */
Product::Product(std::string itemName, int id, double price, int quantity, std::string description)
    : Item(itemName, id)
{
    _price = price;                                                   // initialize price
    _quantity = quantity;                                             // initialize quantity
    strncpy(_description, description.c_str(), sizeof(_description)); // to copy the string description
    _description[sizeof(_description) - 1] = '\0';
}

// Function to generate unique ID for the product
// Implementation can be added as per requirements
int Product::genetateId()
{
    // Implementation for generating unique ID can be added here
    invFile.open(inventoryFilename, std::ios::in | std::ios::binary); // open inventory file in reading in binary mode
    if (!invFile)                                                     // checks if inventory file exists
    {
        std::cerr << "Error opening file for generating ID!" << std::endl; // display error message
        return 1;                                                          // return 1 as default id
    }
    int maxId = 0;
    Product temp;
    while (invFile.read((char *)&temp, sizeof(Product))) // read inventory file
    {
        if (temp._id > maxId)
            maxId = temp._id;
    }
    invFile.close();  // close the file
    return maxId + 1; // return next unique ID
}

/*function to add items to the inventory
which gets user input and stores the product data in a binary file */
void Product::addItem()
{
    std::cout << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear input buffer
    std::cout << "Enter item details:" << std::endl;
    std::cout << "-----------------------------------------------------------------------------" << std::endl;
    std::cout << "Enter item name: ";
    std::cin.getline(_itemName, 50); // get user input of item name
    _id = genetateId();              // generates id for the product
    std::cout << "Enter item price: ";
    std::cin >> _price; // get the price of item from user
    std::cout << "Enter item quantity: ";
    std::cin >> _quantity; // get the quantity of the item
    std::cout << "Enter item description: ";
    std::cin.ignore();
    std::cin.getline(_description, 100); // get the description of the item
    std::cout << "-----------------------------------------------------------------------------" << std::endl;
    strncpy(_addedDateTime, getCurrentDateTime().c_str(), sizeof(_addedDateTime));
    _addedDateTime[sizeof(_addedDateTime) - 1] = '\0';

    invFile.open(inventoryFilename, std::ios::app | std::ios::binary); // opening inventory file in append mode
    if (!invFile)
    {
        std::cerr << "Error opening file for adding item!" << std::endl; // displaying error message in case of any error
        return;
    }
    invFile.write((char *)this, sizeof(Product)); // writes the product object in binary format
    invFile.close();                              // closing the file
    std::cout << std::endl;
    std::cout << "Product added successfully!" << std::endl; // displaying success message
    crossSleep(2000);
}

/* function to edit an item in the inventory
it edits by looking at product id
a temp file is created which stores all the inventory file data and the edited one also then it is renamed as inventory file */
void Product::editItem(int editId)
{
    invFile.open(inventoryFilename, std::ios::in | std::ios::binary);       // Open the inventory file for reading in binary mode
    tempFile.open(tempInventoryFilename, std::ios::out | std::ios::binary); // open the temp file for writing in binary mode
    if (!invFile || !tempFile)                                              // check if file exists
    {
        std::cerr << "Error opening files for editing item!" << std::endl; // display error if inventory or temp file does not exist
        return;
    }
    while (invFile.read((char *)&*this, sizeof(Product))) // read the inventory file
    {
        if (_id == editId) // check if product id is equal to the id given by user
        {
            std::string newName; // initialize new name for the product
            std::cout << "Enter new item name: ";
            std::getline(std::cin >> std::ws, newName); // gets product name from the user to be edited
            std::cout << "Enter new item price: ";
            std::cin >> _price; // gets the price of the product to be changed
            std::cout << "Enter new item quantity: ";
            std::cin >> _quantity; // gets the quantity to be changed
            std::cout << "Enter new item description: ";
            std::cin.ignore();
            std::cin.getline(_description, 100);                    // gets the description of the product to be changed
            strncpy(_itemName, newName.c_str(), sizeof(_itemName)); // copies the changed product name to the item name
            _itemName[sizeof(_itemName) - 1] = '\0';                // ensure null-terminated
        }
        tempFile.write((char *)&*this, sizeof(Product)); // writes product object in temp file
    }
    // closing files
    invFile.close();
    tempFile.close();

    remove(inventoryFilename.c_str());                                // remove old file
    rename(tempInventoryFilename.c_str(), inventoryFilename.c_str()); // rename files

    std::cout << "Product edited successfully!" << std::endl; // success message
}

/* function to delete an item from the inventory
the deleted file is stored in the history file
temp file is made which copies all the data from inventory file expect the data which id is given by the user and remained as inventory file*/
void Product::deleteItem(int deleteId)
{
    invFile.open(inventoryFilename, std::ios::in | std::ios::binary);       // opens inventory file in reading in binary mode
    tempFile.open(tempInventoryFilename, std::ios::out | std::ios::binary); // opens temp file in writing in binary mode
    historyFile.open(historyFilename, std::ios::app | std::ios::binary);    // opens history file in append in binary mode

    if (!invFile || !tempFile || !historyFile) // checks if inventory or temp file or history file exists
    {
        std::cerr << "Error opening files for deleting item!" << std::endl; // display error message
        return;
    }
    while (invFile.read((char *)&*this, sizeof(Product))) // read the inventory file
    {
        if (_id == deleteId) // checks if the product id is equal to the id given by user
        {
            // Log deleted item to history
            historyFile.write((char *)&*this, sizeof(Product));
            std::cout << "Item deleted successfully!" << std::endl;
            continue; // Skip writing to temp file
        }
        tempFile.write((char *)&*this, sizeof(Product)); // writes the product object in binary mode
    }

    // closing files
    invFile.close();
    tempFile.close();
    historyFile.close();

    remove(inventoryFilename.c_str());                                // remove old file
    rename(tempInventoryFilename.c_str(), inventoryFilename.c_str()); // rename files

    std::cout << std::endl;
    std::cout << "Product deleted successfully!" << std::endl; // dispay success message
    crossSleep(2000);
}

/* function to view all data of inventory*/
void Product::viewInv()
{
    invFile.open(inventoryFilename, std::ios::in | std::ios::binary); // open inventory file in reading in binary mode
    if (!invFile)                                                     // checks if inventory file exists
    {
        std::cerr << "Error opening file for viewing inventory!" << std::endl; // displays error message
        return;
    }

    std::cout << std::endl
              << "Inventory:" << std::endl;
    // a table format to display item name, price, quantity and added date
    std::cout << "------------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "| " << std::left << std::setw(4) << "SN"
              << "| " << std::left << std::setw(25) << "Item Name"
              << "| " << std::left << std::setw(8) << "Item ID"
              << "| " << std::setw(20) << "Description"
              << "| " << std::setw(10) << "Price"
              << "| " << std::setw(6) << "Qty"
              << "| " << std::setw(20) << "Added Date"
              << "|" << std::endl;
    std::cout << "------------------------------------------------------------------------------------------------------------" << std::endl;

    Product temp;                                        // a temporary object of product
    int i = 1;                                           // initialize serial number
    while (invFile.read((char *)&temp, sizeof(Product))) // read the inventory file
    {

        std::string d = temp.getTruncatedDesc(18); // get truncated description for display

        // display the data of inventory file in a table format
        std::cout << "| " << std::left << std::setw(4) << i++ // serial number
                  << "| " << std::left << std::setw(25) << temp._itemName
                  << "| " << std::left << std::setw(8) << temp._id
                  << "| " << std::setw(20) << d
                  << "| " << std::setw(10) << std::fixed << std::setprecision(2) << temp._price
                  << "| " << std::setw(6) << temp._quantity
                  << "| " << std::setw(20) << temp._addedDateTime
                  << "|\n";
    }

    std::cout << "------------------------------------------------------------------------------------------------------------" << std::endl;
    invFile.close(); // closing the inventory file
    std::cout << std::endl;
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear input buffer
    std::cin.get();                                                     // pause the program
    std::cout << std::endl;
}

/* function to get truncated description for display */
std::string Product::getTruncatedDesc(int width) const
{
    std::string desc = _description;
    if (desc.length() > width)
    {
        return desc.substr(0, width - 2) + ".."; // Truncate and add ellipsis
    }
    return desc;
}

/* function to view the data of the deleted items
it displays the data stored in history file which stores all the deleted item information*/
void Product::viewDelItem()
{
    historyFile.open(historyFilename, std::ios::in | std::ios::binary); // opens history file in reading in binary mode
    if (!historyFile)                                                   // checks if history file exists
    {
        std::cerr << "Error opening file for viewing history!" << std::endl; // displays error
        return;
    }

    std::cout << std::endl
              << "Deleted Items History:" << std::endl;
    // displays a table of item name, price, quantity and added date
    std::cout << "------------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "| " << std::left << std::setw(4) << "SN"
              << "| " << std::left << std::setw(25) << "Item Name"
              << "| " << std::left << std::setw(8) << "Item ID"
              << "| " << std::setw(20) << "Description"
              << "| " << std::setw(10) << "Price"
              << "| " << std::setw(6) << "Qty"
              << "| " << std::setw(20) << "Added Date"
              << "|\n";
    std::cout << "------------------------------------------------------------------------------------------------------------" << std::endl;

    Product temp;                                            // a temporary product object
    int i = 1;                                               // initialize serial number
    while (historyFile.read((char *)&temp, sizeof(Product))) // reads the history file
    {
        std::string d = temp.getTruncatedDesc(18); // get truncated description for display
        // displays the history information in table
        std::cout << "| " << std::left << std::setw(4) << i++
                  << "| " << std::left << std::setw(25) << temp._itemName
                  << "| " << std::left << std::setw(8) << temp._id
                  << "| " << std::setw(20) << d
                  << "| " << std::setw(10) << std::fixed << std::setprecision(2) << temp._price
                  << "| " << std::setw(6) << temp._quantity
                  << "| " << std::setw(20) << temp._addedDateTime
                  << "|\n";
    }

    std::cout << "------------------------------------------------------------------------------------------------------------" << std::endl;
    historyFile.close(); // close the history file
    std::cout << std::endl;
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear input buffer
    std::cin.get();                                                     // pause the program
    std::cout << std::endl;
}
