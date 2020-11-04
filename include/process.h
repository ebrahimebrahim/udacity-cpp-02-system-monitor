#ifndef PROCESS_H
#define PROCESS_H

#include <string>

#include "linux_parser.h"

class Process {
 public:
  Process() = delete;
  Process(int pid) : pid{pid} {}

  void update(long system_uptime);
  void init();

  int Pid() const {return pid;}
  std::string User() const {return user;}
  std::string Command() const {return cmd;}
  float CpuUtilization() const {return cpu_usage;}
  float RamMB() const {return float(pstat_data.vsize)/1e6;} // "virtual memory" in MB
  long UpTime() const {return long(uptime);}
  bool operator<(Process const& a) const;


 private:
  int pid{};
  LinuxParser::ProcessStatData pstat_data{};
  float cpu_usage{};
  float uptime{};
  std::string user;
  std::string cmd;
  
  unsigned long prev_cpu_ticks{};
  float prev_uptime{};
  
  
};

#endif