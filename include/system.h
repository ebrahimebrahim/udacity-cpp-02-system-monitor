#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <forward_list>

#include "process.h"
#include "processor.h"
#include "linux_parser.h"

class System {
 public:
  Processor& Cpu() { return cpu; }
  std::forward_list<Process>& Processes() { return processes; }
  float MemoryUtilization() const { return memory_utilization; }
  long UpTime() const { return uptime; }
  int TotalProcesses() const { return stat_data.total_processes; }
  int RunningProcesses() const { return stat_data.running_processes; }
  std::string Kernel() const { return kernel; }
  std::string OperatingSystem() const { return operating_system; }

  void init();
  void update();

 private:
  // Set when init() is called
  std::string kernel{};
  std::string operating_system{};

  // Set on every update()
  Processor cpu{};
  std::forward_list<Process> processes{};
  float memory_utilization{};
  long uptime{};
  LinuxParser::StatData stat_data{};
};

#endif