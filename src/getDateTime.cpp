#include "TimeUtils.h"
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

/* Function to get current date and time as formatted string
   Returns: String in format "YYYY-MM-DD HH:MM:SS"
 */
std::string getCurrentDateTime()
{
    auto now = std::chrono::system_clock::now(); // gets current system time
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::tm localTime; // convert to local time structure
#ifdef _WIN32
    localtime_s(&localTime, &t); // for windows
#else
    localtime_r(&t, &localTime); // for mac/linux
#endif

    // format time as string
    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}
