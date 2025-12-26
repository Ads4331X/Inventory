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
            Item *p = new Product();

            p->addItem();
            delete p;
            break;
        }
        case '2':
        { // Edit item logic
            std::cout << "Enter the item ID to edit: ";
            std::cin >> id; // gets the id to edit from the user

            Product P; // object of product

            P.editItem(id); // edits the data from the inventory which id is same as the one given by user
            break;
        }
        case '3':
        { // Delete item logic
            std::cout << "Enter the item ID to delete: ";
            std::cin >> id; // gets the id to delete from the user

            Product P; // object of product

            P.deleteItem(id); // deletes the data from the inventory which id is same as the one given by user
            break;
        }
        case '4':
        {                            // View items logic
            Item *p = new Product(); // object of product

            p->viewInv(); // view all the data of inventory
            delete p;
            break;
        }
        case '5':
        {
            // view deleted history
            Product P; // object of product

            P.viewDelItem(); // to view all the deleted items
            break;
        }
        case '6':
            // Exit logic
            std::cout << std::endl;
            std::cout << "Exiting program..." << std::endl; // display exiting program when exiting the program
            crossSleep(2000);
            break;

        default:
            std::cout << "Invalid choice. Please try again." << std::endl; // Invalid choice handling

            continue;
        }

    } while (choice != '6'); // loops till user choice does not equal to 6
    return 0;
}