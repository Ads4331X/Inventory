#include <iostream>

using namespace std;

int main()
{
    string itemName;
    int id;
    char choice;
    do
    {

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case '1':
        {
            // Add item logic
            cout << "Enter item name: ";
            getline(cin >> ws, itemName);
            cout << "Enter item ID: ";
            cin >> id;
            cout << "Item added successfully!" << endl;
            break;
        }
        case '2':
            // Edit item logic
            int editId;
            cout << "Enter item ID to edit: ";
            cin >> editId;

            break;
        case '3':
            // Delete item logic
            break;
        case '4':
            // View items logic
            break;

        case '6':
        case 'q':
        case 'Q':
            cout << "Exiting program..." << endl;
            break;

        default:
            cout << "Invalid choice. Please try again." << endl; // Invalid choice handling

            continue;
        }

    } while (choice != '6' && choice != 'q' && choice != 'Q');
    return 0;
}