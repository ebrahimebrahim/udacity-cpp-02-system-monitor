#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;


// Can fail quietly if process no longer exists
std::string uid{};
void Process::init() {
  try {
    uid = LinuxParser::Uid(pid);
    cmd = LinuxParser::Command(pid);
  }
  catch (const LinuxParser::fileopen_error & error) {return;} // In this sitution the Process should get deleted soon
  
  user = LinuxParser::User(uid);
}

// Can fail quietly if process no longer exists
void Process::update(long system_uptime) {
  try {
    LinuxParser::ParseProcessStats(pid, pstat_data);
  }
  catch (const LinuxParser::fileopen_error & error) {return;} // In this sitution the Process should get deleted soon

  // Compute uptime for this process
  uptime = float(system_uptime) - float(pstat_data.starttime) / float(sysconf(_SC_CLK_TCK));

  // Compute cpu usage
  const auto d_uptime = uptime - prev_uptime;
  if (d_uptime > 0) {
    const auto total_ticks = pstat_data.utime + pstat_data.stime + pstat_data.cutime + pstat_data.cstime;
    const auto d_ticks = total_ticks - prev_cpu_ticks;
    cpu_usage = (float(d_ticks) / float(sysconf(_SC_CLK_TCK))) / float(d_uptime);
    prev_cpu_ticks = total_ticks;
    prev_uptime = uptime;
  }

}


bool Process::operator<(Process const& a) const {
  return cpu_usage > a.cpu_usage;
  //return uptime < a.uptime;
}

