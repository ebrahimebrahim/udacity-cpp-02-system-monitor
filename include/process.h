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
  std::string User() const;                      // TODO: See src/process.cpp
  std::string Command() const;                   // TODO: See src/process.cpp
  float CpuUtilization() const {return cpu_usage;}                  // TODO: See src/process.cpp
  std::string Ram() const;                       // TODO: See src/process.cpp
  long UpTime() const {return long(uptime);}
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp


  // TODO: Declare any necessary private members
 private:
  int pid{};
  LinuxParser::ProcessStatData pstat_data{};
  float cpu_usage{};
  float uptime{};
  
  unsigned long prev_cpu_ticks{};
  float prev_uptime{};
  
  
};

#endif