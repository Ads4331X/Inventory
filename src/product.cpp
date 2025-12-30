#include "product.h"
#include "../include/sleep.h"
#include <iostream>
#include "Item.h"
#include "config.h"
#include "TimeUtils.h"
#include <iomanip>
#include <fstream>

const int maxNameLength = 49;
const int minNameLength = 1;
const int maxDescLength = 100;
const int minDescLength = 1;

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
int Product::generateId()
{
    // Implementation for generating unique ID can be added here
    invFile.open(inventoryFilename, std::ios::in | std::ios::binary); // open inventory file in reading in binary mode

    int maxId = 0; // initialize max id
    Product temp;  // temporary product object
    if (invFile)   // checks if inventory file exists{
    {
        while (invFile.read((char *)&temp, sizeof(Product))) // read inventory file
        {
            if (temp._id > maxId)
                maxId = temp._id;
        }
        invFile.close(); // close inventory file
    }

    historyFile.open(historyFilename, std::ios::in | std::ios::binary); // open history file in reading in binary mode
    if (historyFile)                                                    // checks if history file exists
    {
        while (historyFile.read((char *)&temp, sizeof(Product))) // read history file
        {
            if (temp._id > maxId) // check for max id
                maxId = temp._id; // update max id
        }
        historyFile.close(); // close history file
    }

    return maxId + 1;
}

/* Validation helper functions */
int Product::validateQuantity()
{
    while (!(std::cin >> _quantity) || _quantity < 0)
    {
        std::cout << "[!] Invalid input. Please enter a numeric quantity: ";
        std::cin.clear();            // Fix the "stuck" state
        std::cin.ignore(1000, '\n'); // Clear invalid input out of the buffer
    }
    std::cin.ignore(1000, '\n'); // Clear the newline character from the buffer
    return _quantity;
}

int Product::validatePrice()
{
    while (!(std::cin >> _price) || _price < 0)
    {
        std::cout << "[!] Invalid input. Please enter a numeric price: ";
        std::cin.clear();            // Fix the "stuck" state
        std::cin.ignore(1000, '\n'); // Clear invalid input out of the buffer
    }
    std::cin.ignore(1000, '\n'); // Clear the newline character from the buffer
    return _price;
}

std::string Product::validateName(int maxLen, int minLen)
{
    std::string name;
    std::getline(std::cin >> std::ws, name);
    while (name.empty() || name.length() > maxLen || name.length() < minLen)
    {
        if (name.length() > maxLen)
            std::cout << "[!] Name too long. Please enter a name up to " << maxLen << " characters: ";
        else if (name.length() < minLen)
            std::cout << "[!] Name too short. Please enter a name with at least " << minLen << " characters: ";
        else
            std::cout << "[!] Name cannot be empty. Please enter a valid name: ";
        std::getline(std::cin, name);
    }
    return name;
}

std::string Product::validateDescription(int maxLen, int minLen)
{
    std::string description;
    std::getline(std::cin >> std::ws, description);
    while (description.length() > maxLen || description.length() < minLen || description.empty())
    {
        if (description.length() > maxLen)
            std::cout << "[!] Description too long. Please enter a description up to " << maxLen << " characters: ";
        else if (description.length() < minLen)
            std::cout << "[!] Description too short. Please enter a description with at least " << minLen << " characters: ";
        else
            std::cout << "[!] Description cannot be empty. Please enter a valid description: ";
        std::getline(std::cin, description);
    }
    return description;
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
    std::string name = validateName(49, 1);              // get the name of item from user by validating it
    strncpy(_itemName, name.c_str(), sizeof(_itemName)); // copy name to _itemName
    _itemName[sizeof(_itemName) - 1] = '\0';             // ensure null-terminated
    _id = genetateId();                                  // generates id for the product
    std::cout << "Enter item price: ";
    _price = validatePrice(); // get the price of item from user by validating it
    std::cout << "Enter item quantity: ";
    _quantity = validateQuantity(); // get the quantity of the item by validating it
    std::cout << "Enter item description: ";
    std::string desc = validateDescription(100, 1);            // validate description
    strncpy(_description, desc.c_str(), sizeof(_description)); // copy description to _description
    _description[sizeof(_description) - 1] = '\0';             // ensure null-terminated
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
    invFile.open(inventoryFilename, std::ios::in | std::ios::binary); // opens inventory file in reading in binary mode
    if (checkFileEmpty(invFile))                                      // checks if inventory file is empty
    {
        std::cout << "\n[!] Inventory is empty. No items to edit.\n\nPress Enter to return...\n";
        if (invFile.is_open())
            invFile.close();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return;
    }

    tempFile.open(tempInventoryFilename, std::ios::out | std::ios::binary); // opens temp file in writing in binary mode
    if (!tempFile)                                                          // check if temp file is created
    {
        std::cerr << "Error: Could not create temporary file for editing." << std::endl; // display error message
        invFile.close();
        return;
    }
    bool found = false;
    Product temp;

    while (invFile.read((char *)&temp, sizeof(Product))) // read the inventory file
    {
        if (temp._id == editId) // checks if product id is equal to the id given by user
        {
            found = true; // set found flag to true

            std::cout << "Enter new name: ";
            std::string newName = validateName(maxNameLength, minNameLength); // validate new name
            strcpy(temp._itemName, newName.c_str());                          // copy new name to item name
            temp._itemName[49] = '\0';                                        // ensure null-terminated
            std::cout << "Enter new price: ";
            temp._price = validatePrice(); // get new price from user by validating it
            std::cout << "Enter new quantity: ";
            temp._quantity = validateQuantity(); // get new quantity from user by validating it
            std::cout << "Enter new description: ";
            std::string newDesc = validateDescription(maxDescLength, minDescLength); // validate new description
            strncpy(temp._description, newDesc.c_str(), sizeof(temp._description));  // copy new description to _description
            temp._description[sizeof(temp._description) - 1] = '\0';                 // ensure null-terminated
        }
        tempFile.write((char *)&temp, sizeof(Product)); // writes the product object in binary mode
    }

    // closing files
    invFile.close();
    tempFile.close();

    if (found)
    {
        remove(inventoryFilename.c_str());                                // remove old file
        rename(tempInventoryFilename.c_str(), inventoryFilename.c_str()); // rename files
        std::cout << "\n[✓] Product updated successfully!\n\n";           // display success message
        std::cout << "Press Enter to continue...";
        std::cin.get();
    }
    else
    {
        remove(tempInventoryFilename.c_str());                           // remove temp file if no changes made
        std::cout << "\n[!] Product ID " << editId << " not found.\n\n"; // display not found message
        std::cout << "Press Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
}

/* function to delete an item from the inventory
the deleted file is stored in the history file
temp file is made which copies all the data from inventory file expect the data which id is given by the user and remained as inventory file*/
void Product::deleteItem(int deleteId)
{
    invFile.open(inventoryFilename, std::ios::in | std::ios::binary); // opens inventory file in reading in binary mode

    if (checkFileEmpty(invFile)) // checks if inventory file is empty
    {
        std::cout << "\n[!] Inventory is empty.\n\nPress Enter to return...\n";
        if (invFile.is_open())
            invFile.close();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return;
    }

    tempFile.open(tempInventoryFilename, std::ios::out | std::ios::binary); // opens temp file in writing in binary mode
    historyFile.open(historyFilename, std::ios::app | std::ios::binary);    // opens history file in append mode
    if (!tempFile || !historyFile)                                          // checks if temp file or history file is opened
    {
        std::cerr << "Error opening file for deleting item!" << std::endl; // display error message
        invFile.close();
        return;
    }

    Product temp;                                        // temporary product object
    bool found = false;                                  // flag to check if item is found
    while (invFile.read((char *)&temp, sizeof(Product))) // read the inventory file
    {
        if (temp._id == deleteId) // checks if product id is equal to the id given by user
        {
            found = true;                                                       // set found flag to true
            strncpy(temp._addedDateTime, getCurrentDateTime().c_str(), 19);     // update deletion date
            temp._addedDateTime[19] = '\0';                                     // ensure null-terminated
            historyFile.write((char *)&temp, sizeof(Product));                  // write deleted item to history file
            std::cout << "\nItem deleted from inventory and stored history.\n"; // display success message
            continue;
        }
        tempFile.write((char *)&temp, sizeof(Product)); // writes the product object in binary mode
    }

    // closing files
    invFile.close();
    tempFile.close();
    historyFile.close();

    if (found)
    {
        remove(inventoryFilename.c_str());                                // remove old inventory file
        rename(tempInventoryFilename.c_str(), inventoryFilename.c_str()); // rename temp file to inventory file
        std::cout << "\n[✓] Product deleted successfully!\n";             // display success message
    }
    else
    {
        remove(tempInventoryFilename.c_str());                             // remove temp file if no changes made
        std::cout << "\n[!] Product ID " << deleteId << " not found.\n\n"; // display not found message
    }

    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

/* function to view all data of inventory*/
void Product::viewInv()
{
    invFile.open(inventoryFilename, std::ios::in | std::ios::binary); // opens inventory file in reading in binary mode
    if (!invFile || checkFileEmpty(invFile))                          // checks if inventory file exists or is empty
    {
        std::cout << "\n[!] Inventory is currently empty. Nothing to display." << std::endl;
        if (invFile.is_open())
            invFile.close();

        std::cout << "\nPress Enter to return to menu...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return;
    }

    // in proper format display the inventory data
    std::cout << "\n========================================= INVENTORY LIST ====================================================" << std::endl;
    std::cout << "| " << std::left << std::setw(4) << "SN"
              << "| " << std::left << std::setw(25) << "Item Name"
              << "| " << std::left << std::setw(8) << "ID"
              << "| " << std::left << std::setw(20) << "Description"
              << "| " << std::left << std::setw(10) << "Price"
              << "| " << std::left << std::setw(6) << "Qty"
              << "| " << std::left << std::setw(20) << "Added Date"
              << " |" << std::endl;
    std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;

    Product temp;      // temporary product object
    int serialNum = 1; // initialize serial number

    while (invFile.read((char *)&temp, sizeof(Product))) // reads the inventory file
    {
        std::string shortDesc = temp.getTruncatedDesc(18); // get truncated description for display

        // displays the inventory data in table format
        std::cout << "| " << std::left << std::setw(4) << serialNum++
                  << "| " << std::left << std::setw(25) << temp._itemName
                  << "| " << std::left << std::setw(8) << temp._id
                  << "| " << std::left << std::setw(20) << shortDesc
                  << "| $" << std::fixed << std::setprecision(2) << std::setw(9) << temp._price
                  << "| " << std::setw(6) << temp._quantity
                  << "| " << std::setw(20) << temp._addedDateTime
                  << " |" << std::endl;
    }

    std::cout << "=============================================================================================================" << std::endl;

    invFile.close(); // close the inventory file

    std::cout << "\nEnd of list. Press Enter to continue...";

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
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

    if (!historyFile || checkFileEmpty(historyFile)) // checks if history file exists or is empty
    {
        std::cout << "\n[!] Deleted Items History is currently empty." << std::endl; // display message
        if (historyFile.is_open())
            historyFile.close();

        std::cout << "\nPress Enter to return to menu...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear input buffer
        std::cin.get();
        return;
    }

    // display the deleted items in proper format
    std::cout << "\n============================== DELETED ITEMS HISTORY ========================================================" << std::endl;
    std::cout << "| " << std::left << std::setw(4) << "SN"
              << "| " << std::left << std::setw(25) << "Item Name"
              << "| " << std::left << std::setw(8) << "ID"
              << "| " << std::left << std::setw(20) << "Description"
              << "| " << std::left << std::setw(10) << "Price"
              << "| " << std::left << std::setw(6) << "Qty"
              << "| " << std::left << std::setw(20) << "Deleted Date"
              << " |" << std::endl;
    std::cout << "------------------------------------------------------------------------------------------------------------" << std::endl;

    Product temp; // temporary product object
    int i = 1;    // initialize serial number
    while (historyFile.read((char *)&temp, sizeof(Product)))
    {
        std::string d = temp.getTruncatedDesc(18); // get truncated description for display

        // displays the deleted items data in table format
        std::cout << "| " << std::left << std::setw(4) << i++
                  << "| " << std::left << std::setw(25) << temp._itemName
                  << "| " << std::left << std::setw(8) << temp._id
                  << "| " << std::left << std::setw(20) << d
                  << "| $" << std::fixed << std::setw(10) << temp._price
                  << "| " << std::setw(6) << temp._quantity
                  << "| " << std::setw(20) << temp._addedDateTime
                  << "|" << std::endl;
    }

    std::cout << "=============================================================================================================" << std::endl;

    historyFile.close(); // close the history file
    std::cout << "\nPress Enter to return to menu...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear input buffer
    std::cin.get();                                                     // wait for user input
}
/* function to display the data of item that user has just entered*/
void Product::display()
{
    std::cout << "\n\t+---------------------------------------------------+" << std::endl;
    std::cout << "\t|                  ITEM DETAILS                     |" << std::endl;
    std::cout << "\t+---------------------------------------------------+" << std::endl;
    std::cout << "\t  ID          : " << _id << std::endl;
    std::cout << "\t  Name        : " << _itemName << std::endl;
    std::cout << "\t  Price       : $" << std::fixed << std::setprecision(2) << _price << std::endl;
    std::cout << "\t  Quantity    : " << _quantity << std::endl;
    std::cout << "\t  Added Date  : " << _addedDateTime << std::endl;
    std::cout << "\t  Description : " << _description << std::endl;
    std::cout << "\t+---------------------------------------------------+" << std::endl;
}

/* function to search a product */
/* function to search a product */
void Product::searchItem()
{
    // Check if the previous input left a newline in the buffer
    if (std::cin.peek() == '\n')
    {
        std::cin.ignore();
    }

    invFile.open(inventoryFilename, std::ios::in | std::ios::binary); // open inventory file in reading in binary mode
    if (!invFile || checkFileEmpty(invFile))                          // checks if inventory file exists or is empty
    {
        std::cout << "\n[!] Inventory is currently empty. No items to search." << std::endl;
        if (invFile.is_open())
            invFile.close();
        std::cout << "\nPress Enter to return to menu...";
        std::cin.get();
        return;
    }

    std::string name; // variable to store the name to search
    std::cout << "\nEnter the item name to search: ";
    name = validateName(49, 1); // validate and get the name to search from user

    Product temp;
    bool found = false;

    std::cout << "\n--- Searching for: \" " << name << " \" ---" << std::endl;

    while (invFile.read((char *)&temp, sizeof(Product))) // read the inventory file
    {
        if (std::string(temp._itemName).find(name) != std::string::npos) // check if item name contains the search string
        {
            temp.display(); // display the product details
            found = true;   // set found flag to true
        }
    }

    if (!found) // if no matching items found
    {
        std::cout << "\n[!] No results found for \"" << name << "\"" << std::endl; // display not found message
    }

    invFile.close(); // close the inventory file
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}
/* function to generate report (total sale) */
double Product::generateReport()
{
    invFile.open(inventoryFilename, std::ios::in | std::ios::binary); // open inventory file in reading in binary mode

    if (!invFile || checkFileEmpty(invFile)) // checks if inventory file exists or is empty
    {
        std::cout << "\n[!] Inventory is currently empty. No report can be generated." << std::endl;
        if (invFile.is_open())
            invFile.close();

        std::cout << "\nPress Enter to return to menu...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return 0.0;
    }

    double grandTotal = 0.0;
    Product temp;

    // Display report header
    std::cout << "\n==========================================" << std::endl;
    std::cout << "        INVENTORY VALUATION REPORT        " << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << std::left << std::setw(25) << "Item Name" << " | " << "Value" << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    while (invFile.read((char *)&temp, sizeof(Product)))
    {
        double itemValue = temp._price * temp._quantity; // calculate item's total value

        // Print it as a report line
        std::cout << std::left << std::setw(25) << temp._itemName
                  << " | $" << std::fixed << std::setprecision(2) << itemValue << std::endl;

        grandTotal += itemValue; // accumulate to grand total
    }

    std::cout << "------------------------------------------" << std::endl;
    std::cout << "GRAND TOTAL VALUE:        $" << grandTotal << std::endl; // display grand total
    std::cout << "==========================================" << std::endl;

    invFile.close(); // close the inventory file

    std::cout << "\nPress Enter to return to menu...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();

    return grandTotal;
}

/* function to check if file is empty */
bool Product::checkFileEmpty(std::fstream &file)
{
    if (!file.is_open())
        return true;
    file.clear();                 // Clear any potential flags
    file.seekg(0, std::ios::beg); // Reset pointer to the start
    return file.peek() == std::fstream::traits_type::eof();
}