#include <iostream>
#include "product.h"

int main()
{
    std::string itemName;
    int id;
    char choice;
    Product P;

    do
    {

        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice)
        {
        case '1':
        {
            P.addItem();
            break;
        }
        case '2':
            // Edit item logic
            std::cout << "Enter the item ID to edit: ";
            std::cin >> id;

            P.editItem(id);
            break;
        case '3':
            // Delete item logic
            std::cout << "Enter the item ID to delete: ";
            std::cin >> id;

            P.deleteItem(id);
            break;
        case '4':
            // View items logic
            break;

        case '6':
        case 'q':
        case 'Q':
            std::cout << "Exiting program..." << std::endl;
            break;

        default:
            std::cout << "Invalid choice. Please try again." << std::endl; // Invalid choice handling

            continue;
        }

    } while (choice != '6' && choice != 'q' && choice != 'Q');
    return 0;
}