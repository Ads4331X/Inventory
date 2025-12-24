#include <iostream>
#include "product.h"
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
            Product P; // object of product

            P.addItem();
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
        {              // View items logic
            Product P; // object of product

            P.viewInv(); // view all the data of inventory
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
            std::cout << "Exiting program..." << std::endl; // display exiting program when exiting the program
            break;

        default:
            std::cout << "Invalid choice. Please try again." << std::endl; // Invalid choice handling

            continue;
        }

    } while (choice != '6'); // loops till user choice does not equal to 6
    return 0;
}