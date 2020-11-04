#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;


// TODO: update as needed
// Can fail quietly if process no longer exists
void Process::init() {}

// TODO: update as needed
// Can fail quietly if process no longer exists
void Process::update() {
  try {
    LinuxParser::ParseProcessStats(pid, pstat_data);
  }
  catch (const LinuxParser::fileopen_error & error) {} // In this sitution the Process should get deleted soon
}




// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { return 0; }

// TODO: Return the command that generated this process
string Process::Command() const { return string(); }

// TODO: Return this process's memory utilization
string Process::Ram() const { return string(); }

// TODO: Return the user (name) that generated this process
string Process::User() const { return string(); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const { return 0; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a [[maybe_unused]]) const {
  return true;
}

