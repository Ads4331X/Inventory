# Inventory Management System

## Project Overview

A C++ console-based inventory management system that allows users to efficiently manage product inventory with comprehensive CRUD (Create, Read, Update, Delete) operations. The system implements Object-Oriented Programming principles including inheritance, polymorphism, and encapsulation, with persistent data storage using binary file handling.

## Features

### Core Functionality

- **Add Products**: Create new inventory items with details (name, price, quantity, description)
- **Auto-ID Generation**: Automatically generates unique product IDs by scanning both active and deleted records
- **Edit Products**: Update existing product information by ID
- **Delete Products**: Remove items from inventory (automatically moved to history for record-keeping)
- **View Inventory**: Display all active products in formatted table view
- **View History**: Access deleted items history with deletion timestamps

### Advanced Features

- **Smart Search**: Search products by name with partial matching support
- **Inventory Reports**: Generate valuation reports showing total inventory worth
- **Input Validation**: Robust error handling and input validation throughout
- **Empty State Handling**: Graceful handling of empty inventory or history files

### Technical Features

- **Binary File Storage**: Efficient data persistence using binary file operations
- **Cross-Platform Sleep**: Compatible sleep function for Windows, Linux, and macOS
- **Memory Management**: Proper dynamic memory allocation and deallocation
- **OOP Design**: Demonstrates inheritance, polymorphism, virtual functions, and encapsulation

## Technologies Used

- **Language**: C++ (C++11 or higher)
- **Compiler**: g++ (GNU Compiler Collection)
- **Build System**: VSCode tasks configuration
- **File I/O**: Binary file handling with fstream
- **Platform**: Cross-platform (Windows, Linux, macOS)

## File Structure

```
├── include/                  # Header files
│   ├── Item.h               # Base class declaration
│   ├── product.h            # Product class (derived from Item)
│   ├── config.h             # File path configurations
│   └── sleep.h              # Cross-platform sleep utility
├── src/                      # Source files
│   ├── inventory.cpp        # Main program with menu system
│   ├── product.cpp          # Product class implementation
│   ├── Item.cpp             # Item base class implementation
│   ├── showMenu.cpp         # Menu display and validation
│   ├── config.cpp           # Configuration constants
│   ├── getDateTime.cpp      # Date/time utility
│   └── sleep.cpp            # Sleep implementation
├── data/                     # Data storage directory
│   ├── Inventory.txt        # Active inventory (binary)
│   ├── History.txt          # Deleted items log (binary)
│   └── TempInventory.txt    # Temporary file for operations
└── .vscode/                  # VSCode configuration
    ├── tasks.json           # Build tasks
    └── launch.json          # Debug configuration
```

## How to Compile

### Using VSCode (Recommended)

1. Open the project folder in Visual Studio Code
2. Press `F5` or click "Run and Debug"
3. The build task will automatically compile all source files

### Using Command Line

**Linux/macOS:**

```bash
g++ -std=c++11 src/*.cpp -I include -o inventory_program
./inventory_program
```

**Windows:**

```cmd
g++ -std=c++11 src/*.cpp -I include -o inventory_program.exe
inventory_program.exe
```

## How to Run

1. Ensure you have a C++ compiler (g++) installed on your system
2. The `data/` folder must exist in the root directory for storing binary files
3. Compile the project using one of the methods above
4. Run the executable
5. Follow the on-screen menu to interact with the inventory system

## System Requirements

- C++11 compatible compiler
- Minimum 1 MB free disk space
- Terminal/Console window
- Operating System: Windows, Linux, or macOS

## Menu Options

1. **Add Item** - Add a new product to inventory
2. **Edit Item** - Modify existing product by ID
3. **Delete Item** - Remove product (moves to history)
4. **View Items** - Display all inventory in table format
5. **View Deleted History** - Show deleted items log
6. **Search Item** - Find products by name
7. **Generate Report** - View inventory valuation report
8. **Exit** - Close the application

## Object-Oriented Design

### Class Hierarchy

- **Item** (Abstract Base Class)
  - Contains common attributes: name, ID, date/time
  - Declares pure virtual functions for polymorphism
- **Product** (Derived Class)
  - Inherits from Item
  - Adds product-specific attributes: price, quantity, description
  - Implements all virtual functions from Item

### Key OOP Concepts Demonstrated

- **Inheritance**: Product inherits from Item
- **Polymorphism**: Virtual functions allow runtime binding
- **Encapsulation**: Private data members with public methods
- **Abstraction**: Abstract base class defines interface

## Data Persistence

- Uses binary file format for efficient storage
- Three data files:
  - `Inventory.txt`: Current active inventory
  - `History.txt`: Log of deleted items
  - `TempInventory.txt`: Temporary file for edit/delete operations

## Error Handling

- File operation error checking
- Input validation for numeric fields
- Empty inventory/history state handling
- Graceful handling of non-existent IDs

## Known Limitations

- Single-user system (no concurrent access support)
- Binary files are not human-readable
- No data encryption
- Limited to local file system storage

## Future Enhancements

- Multi-user support with authentication
- Database integration (SQLite/MySQL)
- GUI interface using Qt or similar framework
- Export to CSV/Excel functionality
- Barcode/QR code scanning support

## Author

**Adarsha Karki**  
Student ID: 25068584  
Date: 2nd January, 2026

## License

This project is created for educational purposes as part of the Introduction to Programming course (UFCFM1-15-0).

## Acknowledgments

- Course: Introduction to Programming
- Module Code: UFCFM1-15-0
- Level: 3
- Assessment: 60% of total module mark
