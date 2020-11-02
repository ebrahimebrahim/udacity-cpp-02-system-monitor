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


void System::init() {
    kernel = LinuxParser::Kernel();
    operating_system = LinuxParser::OperatingSystem();
    
    update();
}


void System::update() {
    cpu.update();

    for (Process & process : processes)
        process.update();
    
    memory_utilization = LinuxParser::MemoryUtilization();
    uptime = LinuxParser::UpTime();
    total_processes = LinuxParser::TotalProcesses();
    running_processes = LinuxParser::RunningProcesses();
}