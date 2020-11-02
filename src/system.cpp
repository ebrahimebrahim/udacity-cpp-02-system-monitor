#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;


// TODO: set appropriate member variables using LinuxParser
void System::init() {
    memory_utilization = LinuxParser::MemoryUtilization();
}


// TODO: update appropriate member variables using LinuxParser
void System::update() {
    memory_utilization = LinuxParser::MemoryUtilization();
}