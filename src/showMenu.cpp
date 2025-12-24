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

    char choice; // initialize a character in which takes user input

    // displays the choices that the user can enter in a proper format
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
    std::cin >> choice;            // gets the user choice
    return validateChoice(choice); // Validate choice
}

/* function to check if the choice that user has given is valid or nor*/
char validateChoice(char choice)
{
    if (choice >= '1' && choice <= '6') // checks if choise lies within 1 to 6
        return choice;                  // return the choice

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
