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

// Struct to represent what gets parsed from /proc/stat
struct StatData {
  // CPU info; see /proc/stat in "man proc"
  using cpu_time_type = unsigned int;
  cpu_time_type user_time{}; // Time spent in user mode
  cpu_time_type nice_time{};   // Time spent in user mode with low priority (nice).
  cpu_time_type system_time{}; // Time spent in system mode.
  cpu_time_type idle_time{};   // Time spent in the idle task.  This value should be USER_HZ times the second entry in the /proc/uptime pseudo-file.
  cpu_time_type iowait_time{}; // Time waiting for I/O to complete.  This value is not reliable, for the following reasons:
  cpu_time_type irq_time{};    // Time servicing interrupts.
  cpu_time_type softirq_time{}; // Time servicing softirqs.
  cpu_time_type steal_time{}; // Stolen time, which is the time spent in other operating systems when running in a virtualized environment
  cpu_time_type guest_time{}; // Time spent running a virtual CPU for guest operating systems under the control of the Linux kernel.
  cpu_time_type guest_nice_time{}; // Time spent running a niced guest (virtual CPU for guest operating systems under the control of the Linux kernel).
  
  
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


// CPU
enum CPUStates {  // TODO make this enum class for better type checking
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};

// Processes
long ActiveJiffies(int pid);
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
};  // namespace LinuxParser

#endif