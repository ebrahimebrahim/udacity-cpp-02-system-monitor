#include <dirent.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>

#include "linux_parser.h"
#include "util.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

#define FAIL_STRING "[PARSER ERROR]"


// Open file and return filestream, throwing an exception if it doesn't work out
std::ifstream LinuxParser::try_open(const std::string & filepath) {
  std::ifstream ifs(filepath);
  if (!ifs.is_open()) throw std::runtime_error(std::string("Unable to open file ")+filepath);
  return ifs;
}


// Parse OS pretty name
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream ifs = try_open(kOSPath);
  while (std::getline(ifs, line)) {
    auto split_line = split(line, "=");
    if (split_line.size() >= 2 && split_line[0] == "PRETTY_NAME")
      return strip(split_line[1], {'\"'});
  }
  return FAIL_STRING;
}


// Parse linux kernl version
string LinuxParser::Kernel() {
  string line;
  std::ifstream ifs = try_open(kProcDirectory + kVersionFilename);
  std::getline(ifs, line);
  auto split = split_whitespace(line);
  if (split.size() < 3)
    return FAIL_STRING;
  else
    return split[2];
}


// BONUS: Update this to use std::filesystem
std::unordered_set<int> LinuxParser::Pids() {
  std::unordered_set<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.insert(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}


// Parse /proc/meminfo to estimate memory utilization
// This considers available memory to be MemAvailable, described here:
// https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/commit/?id=34e431b0ae398fc54ea69ff85ec700722c9da773 
// Perhaps this program ought to have a fallback in case MemAvailable isn't there, for compatibility with older linux.
float LinuxParser::MemoryUtilization() {
  string token;
  std::ifstream ifs = try_open(kProcDirectory + kMeminfoFilename);
  long memtotal = -1;
  long memfree = -1;
  while (ifs >> token && (memtotal < 0 || memfree < 0)){
    if (token=="MemTotal:") {
      if (! (ifs >> memtotal) ) break;
    }
    else if (token=="MemAvailable:") {
      if (! (ifs >> memfree) ) break;
    }
  }
  if (memtotal <= 0 || memfree < 0)
    throw std::runtime_error(std::string("Error parsing MemTotal and MemAvailable in ")+kProcDirectory + kMeminfoFilename);
  
  return float(memtotal - memfree)/float(memtotal);
 }


// Parse system uptime from /proc/uptime
long LinuxParser::UpTime() {
  std::ifstream ifs = try_open(kProcDirectory + kUptimeFilename);
  long uptime = 0;
  if (! (ifs >> uptime) )
    throw std::runtime_error(std::string("Error parsing uptime in ")+kProcDirectory + kUptimeFilename);
  return uptime;
}


// Parse cpu data and process count data from /proc/stat
// Store the result in the argument stat_data
void LinuxParser::ParseStats(LinuxParser::StatData & stat_data) {
  std::ifstream ifs = try_open(kProcDirectory + kStatFilename);
  std::string line;
  uint_fast8_t success_flags = 0b0000'0000;
  while(std::getline(ifs,line) && success_flags!=0b0000'0111) {
    auto line_split = split_whitespace(line);
    
    if (line_split.empty()) continue;
    
    if (line_split[0] == "cpu" && line_split.size() >= 1+NUM_CPU_STATES ) {
      if (stat_data.cpu_times.size() < NUM_CPU_STATES) throw std::logic_error("stat_data size incorrect!");
      try {
        for (int i=0; i < NUM_CPU_STATES; ++i)
          stat_data.cpu_times[i] = std::stoi(line_split[i+1]);
      }
      catch (...) {break;} // If there is a conversion error, then just don't set success flag and probably throw the exception below 
      success_flags |= 0b0000'0001;
    }

    else if (line_split[0] == "processes" && line_split.size()>=2) {
      try {stat_data.total_processes = std::stoi(line_split[1]);}
      catch (...) {break;} 
      success_flags |= 0b0000'0010;
    }

    else if (line_split[0] == "procs_running" && line_split.size()>=2) {
      try {stat_data.running_processes = std::stoi(line_split[1]);}
      catch (...) {break;}
      success_flags |= 0b0000'0100;
    }

  }

  if (success_flags != 0b0000'0111)
    throw std::runtime_error(std::string("Error parsing ")+kProcDirectory + kStatFilename);
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }