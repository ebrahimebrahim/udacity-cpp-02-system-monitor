#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

void System::init() {
  kernel = LinuxParser::Kernel();
  operating_system = LinuxParser::OperatingSystem();

  auto pids = LinuxParser::Pids();
  for (auto const & pid : pids) {
    processes.emplace_back(pid);
    processes.back().init();
  }

  update();
}

void System::update() {
  memory_utilization = LinuxParser::MemoryUtilization();
  uptime = LinuxParser::UpTime();
  LinuxParser::ParseStats(stat_data);
  
  cpu.update(stat_data);
  for (Process& process : processes) process.update();
}