#include "../include/config.h"

std::fstream invFile;     // for inventory file
std::fstream tempFile;    // for temp file used for deleting or editing the data in inventory file
std::fstream historyFile; // for storing the deleted data of inventory

const std::string inventoryFilename = "data/Inventory.txt";         // location of inventory file
const std::string tempInventoryFilename = "data/TempInventory.txt"; // location of temp file
const std::string historyFilename = "data/History.txt";             // location of history file