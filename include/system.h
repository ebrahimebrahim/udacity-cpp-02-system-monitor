#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu() { return cpu; }
  std::vector<Process>& Processes() { return processes; }
  float MemoryUtilization() const { return memory_utilization; }
  long UpTime() const { return uptime; }
  int TotalProcesses() const { return total_processes; }
  int RunningProcesses() const { return running_processes; }
  std::string Kernel() const { return kernel; }
  std::string OperatingSystem() const { return operating_system; }

  void init();
  void update();

 private:
  // Set when init() is called
  std::string kernel{};
  std::string operating_system{};

  // Set on every update()
  Processor cpu = {};
  std::vector<Process> processes = {};
  float memory_utilization{};
  long uptime{};
  int total_processes{};
  int running_processes{};
};

#endif