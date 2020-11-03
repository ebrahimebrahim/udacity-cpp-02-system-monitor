#ifndef PROCESS_H
#define PROCESS_H

#include <string>

#include "linux_parser.h"

class Process {
 public:
  Process() = delete;
  Process(int pid) : pid{pid} {}

  void update();
  void init();

  int Pid() const {return pid;}
  std::string User() const;                      // TODO: See src/process.cpp
  std::string Command() const;                   // TODO: See src/process.cpp
  float CpuUtilization() const;                  // TODO: See src/process.cpp
  std::string Ram() const;                       // TODO: See src/process.cpp
  long int UpTime() const;                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp


  // TODO: Declare any necessary private members
 private:
  int pid{};
  LinuxParser::ProcessStatData pstat_data{};
  
};

#endif