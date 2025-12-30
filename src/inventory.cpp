#include <iostream>
#include "../include/product.h"
#include "../include/config.h"
#include "../include/sleep.h"
#include "showMenu.h"

int main()
{
    std::string itemName;
    int id;
    char choice;

    do
    {

        choice = showMenu();
        if (choice == '\0')
            continue;
        switch (choice)
        {
        case '1':
        {
            // We instantiate a derived 'Product' object through a base 'Item' pointer.
            // This allows the program to call the correct 'addItem' implementation at runtime.
            Item *p = new Product();

            p->addItem();
            delete p; // Manual memory management to prevent memory leaks
            break;
        }
        case '2':
        {
            // Edit item logic
            // Identifies a specific record in the binary file by its unique ID
            // and allows modification of attributes without affecting other records.
            std::cout << "Enter the item ID to edit: ";
            while (!(std::cin >> id))
            {
                std::cout << "[!] Invalid input. Please enter a numeric ID: ";
                std::cin.clear();            // Fix the "stuck" state
                std::cin.ignore(1000, '\n'); // Clear the 'abc' out of the buffer
            }

            Item *P = new Product();

            P->editItem(id); // edits the data from the inventory which id is same as the one given by user
            delete P;        // Manual memory management to prevent memory leaks
            break;
        }
        case '3':
        { // Delete item logic
            std::cout << "Enter the item ID to delete: ";
            while (!(std::cin >> id)) // Validate input
            {
                std::cout << "[!] Invalid input. Please enter a numeric ID: ";
                std::cin.clear();
                std::cin.ignore(1000, '\n');
            }

            Item *P = new Product();

            P->deleteItem(id); // deletes the data from the inventory which id is same as the one given by user
            delete P;          // Manual memory management to prevent memory leaks
            break;
        }
        case '4':
        {                            // View items logic
            Item *p = new Product(); // object of product

            p->viewInv(); // view all the data of inventory
            delete p;     // Manual memory management to prevent memory leaks
            break;
        }
        case '5':
        {
            // view deleted history
            Item *P = new Product();

            P->viewDelItem(); // to view all the deleted items
            delete P;         // Manual memory management to prevent memory leaks
            break;
        }
        case '6':
        {

            Item *P = new Product();
            P->searchItem();
            delete P;
            break;
        }
        case '7':
        {
            // Generate report logic
            Item *P = new Product();
            P->genetateReport();
            delete P;
            break;
        }
        case '8':
            // Exit logic
            // Provides feedback to the user and pauses the execution
            std::cout << std::endl;
            std::cout << "Exiting program..." << std::endl; // display exiting program when exiting the program
            crossSleep(2000);                               // pause for 2 seconds
            break;

        default:
            std::cout << "Invalid choice. Please try again." << std::endl; // Invalid choice handling

            continue;
        }

    } while (choice != '8'); // loops till user choice does not equal to 6
    return 0;
}