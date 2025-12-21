#include "showMenu.h"

#include <iostream>

#ifdef _WIN32
#include <windows.h> // for Sleep()
#else
#include <unistd.h> // for sleep()
#endif

char ShowMenu()
{
    // Clear the console
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    char choice;

    std::cout << "Inventory Management System" << std::endl;
    std::cout << "------------------------------" << std::endl;
    std::cout << "1. Add Item" << std::endl;
    std::cout << "2. Edit Item" << std::endl;
    std::cout << "3. Delete Item" << std::endl;
    std::cout << "4. View Items" << std::endl;
    std::cout << "5. View Deleted History" << std::endl;
    std::cout << "6. Exit/Q" << std::endl;
    std::cout << "------------------------------" << std::endl;

    std::cout << "Enter your choice: ";
    std::cin >> choice;
    return validateChoice(choice); // Validate choice
}

char validateChoice(char choice)
{
    if (choice >= '1' && choice <= '6')
        return choice;

    else
    {
        std::cout << "Invalid choice. Please try again." << std::endl; // Invalid choice handling

        // Pause for 2 seconds
#ifdef _WIN32
        Sleep(2000); // sleep for Windows;
#else
        sleep(2); // sleep for linux and macOS
#endif
    }
    return '\0'; // Return null character for invalid choice
}
