#include <string>
#include <sstream>
#include <iomanip>

#include "format.h"


// OUTPUT: HH:MM:SS
std::string Format::ElapsedTime(long seconds) { 
    std::ostringstream oss;
    oss.flags(std::ios::right);
    oss.fill('0');
    oss << std::setw(2) << seconds/3600 << ':'      // HH
        << std::setw(2) << (seconds%3600)/60 << ':' // MM
        << std::setw(2) << seconds%60;              // DD
    return oss.str();
}