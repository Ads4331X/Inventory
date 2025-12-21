#ifndef CONFIG_h
#define CONFIG_h

#include <fstream>
#include <string>

extern std::fstream invFile;
extern std::fstream tempFile;
extern std::fstream historyFile;

extern const std::string inventoryFilename;
extern const std::string tempInventoryFilename;
extern const std::string historyFilename;

#endif