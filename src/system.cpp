#include <unistd.h>
#include <cstddef>
#include <string>
#include <unordered_set>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::size_t;
using std::string;

void System::init() {
  kernel = LinuxParser::Kernel();
  operating_system = LinuxParser::OperatingSystem();

  update();
}

void System::update() {
  memory_utilization = LinuxParser::MemoryUtilization();
  uptime = LinuxParser::UpTime();
  LinuxParser::ParseStats(stat_data);
  cpu.update(stat_data);

  // prune processes that have disappeared
  const auto pids = LinuxParser::Pids();
  processes.remove_if([&pids](const Process & process) {
    return pids.find(process.Pid())==pids.end();
  });

  // create new processes that have showed up
  std::unordered_set<int> new_processes{};
  for (auto const & pid : pids){
    if (prev_pids.find(pid)==prev_pids.end()){ // if pid is in pids but not prev_pids
      processes.emplace_front(pid);
      processes.front().init(); // can fail quietly if process disappeared, but then this proc will be removed at the next update()
    }
  }

  // done using updated pids list
  prev_pids = std::move(pids);

  // update all processes
  for (auto & process : processes)
    process.update(uptime);

  // sort processes every n updates
  if (update_count%10==0)
    processes.sort();

  ++update_count;
}