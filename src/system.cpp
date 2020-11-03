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

  const auto pids = LinuxParser::Pids();
  for (auto const & pid : pids) {
    processes.emplace_front(pid);
    processes.front().init();
  }

  update();
}

void System::update() {
  memory_utilization = LinuxParser::MemoryUtilization();
  uptime = LinuxParser::UpTime();
  
  LinuxParser::ParseStats(stat_data);
  
  cpu.update(stat_data);
  for (Process& process : processes) process.update();

  // prune processes that have disappeared
  const auto pids = LinuxParser::Pids();
  processes.remove_if([&pids](const Process & process) {
    return pids.find(process.Pid())==pids.end();
  });
}