#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>
#include <unordered_set>
#include <stdexcept>

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
std::string pid_directory(int pid);
std::ifstream try_open(const std::string & filepath);

// Exception thrown for file opening errors
class fileopen_error : public std::runtime_error {
    using runtime_error::runtime_error;
};

// Exception thrown for parsing errors
class parse_error : public std::runtime_error {
    using runtime_error::runtime_error;
};

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

// Struct to represent what gets parsed from /proc/[pid]/stat
// See "/proc/[pid]/stat" in "man proc" for details
struct ProcessStatData {
  // these are in clock ticks, divide by sysconf(_SC_CLK_TCK) to convert to seconds
  unsigned long utime{}; // Amount of time that this process has been scheduled in user mode
  unsigned long stime{}; // Amount of time that this process has been scheduled in kernel mode
  unsigned long cutime{}; // Amount  of  time  that this process's waited-for children have been scheduled in user mode
  unsigned long cstime{}; // Amount of time that this process's waited-for children have been scheduled in kernel mode
  unsigned long long starttime{}; // The time the process started after system boot
  
  unsigned long vsize{}; // Virtual memory size in bytes
};

// System
float MemoryUtilization();
long UpTime();
void ParseStats(StatData &);
std::unordered_set<int> Pids();
std::string OperatingSystem();
std::string Kernel();

// Processes
void ParseProcessStats(int pid, ProcessStatData &);
std::string Command(int pid);
std::string Uid(int pid);
std::string User(const std::string & uid);





};  // namespace LinuxParser

#endif