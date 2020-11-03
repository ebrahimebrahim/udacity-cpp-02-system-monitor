#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};
std::ifstream try_open(const std::string & filepath);

// CPU states to serve as indices in vectors. See /proc/stat in "man proc"
enum CPUStates {
  CPU_STATE_USER = 0, // user mode
  CPU_STATE_NICE,   // user mode with low priority (nice).
  CPU_STATE_SYSTEM, // system mode.
  CPU_STATE_IDLE,   // idle task.
  CPU_STATE_IOWAIT, // waiting for I/O to complete.
  CPU_STATE_IRQ,    // servicing interrupts.
  CPU_STATE_SOFTIRQ, // servicing softirqs.
  CPU_STATE_STEAL, // in other operating systems when running in a virtualized environment
  CPU_STATE_GUEST, // running a virtual CPU for guest operating systems under the control of the Linux kernel.
  CPU_STATE_GUEST_NICE, // running a niced guest (virtual CPU for guest operating systems under the control of the Linux kernel).
  NUM_CPU_STATES
};

// Struct to represent what gets parsed from /proc/stat
struct StatData {
  StatData() : cpu_times(NUM_CPU_STATES, 0) {}

  // CPU info
  std::vector<unsigned int> cpu_times; // index will be a value of the CPUStates enum above
  
  // Process info
  int total_processes{};
  int running_processes{};
};

// System
float MemoryUtilization();
long UpTime();
void ParseStats(StatData &);
std::vector<int> Pids();
std::string OperatingSystem();
std::string Kernel();

// Processes
long ActiveJiffies(int pid);
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
};  // namespace LinuxParser

#endif