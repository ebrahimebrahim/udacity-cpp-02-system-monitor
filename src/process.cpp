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
void Process::update(long system_uptime) {
  try {
    LinuxParser::ParseProcessStats(pid, pstat_data);
  }
  catch (const LinuxParser::fileopen_error & error) {return;} // In this sitution the Process should get deleted soon

  // Compute uptime for this process
  float uptime = float(system_uptime) - float(pstat_data.starttime) / float(sysconf(_SC_CLK_TCK));

  // Compute cpu usage
  const auto total_ticks = pstat_data.utime + pstat_data.stime + pstat_data.cutime + pstat_data.cstime;
  const auto d_ticks = total_ticks - prev_cpu_ticks;
  const auto d_uptime = uptime - prev_uptime;
  if (d_uptime == 0)
    cpu_usage = 0;
  else
    cpu_usage = 100 * (float(d_ticks) / float(sysconf(_SC_CLK_TCK))) / float(d_uptime);


  prev_cpu_ticks = total_ticks;
  prev_uptime = uptime;
}





// TODO: Return the command that generated this process
string Process::Command() const { return string(); }

// TODO: Return this process's memory utilization
string Process::Ram() const { return string(); }

// TODO: Return the user (name) that generated this process
string Process::User() const { return string(); }


bool Process::operator<(Process const& a [[maybe_unused]]) const {
  return cpu_usage > a.cpu_usage;
}

