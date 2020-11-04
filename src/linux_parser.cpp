#include <filesystem>
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


std::string LinuxParser::pid_directory(int pid) {
  std::ostringstream oss;
  oss << kProcDirectory << '/' << pid;
  return oss.str();
}

// Open file and return filestream, throwing a fileopen_error if it doesn't work out
std::ifstream LinuxParser::try_open(const std::string & filepath) {
  std::ifstream ifs(filepath);
  if (!ifs.is_open()) throw LinuxParser::fileopen_error(std::string("Unable to open file ")+filepath);
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

std::unordered_set<int> LinuxParser::Pids() {
  std::unordered_set<int> pids;
  for (const auto & dir_entry : std::filesystem::directory_iterator(kProcDirectory)){
    std::ostringstream oss;
    oss << dir_entry.path().filename();
    std::string filename = strip(oss.str(),{'\"'});
    if (std::all_of(filename.begin(), filename.end(), isdigit)) {
      int pid = stoi(filename);
      pids.insert(pid);
    }
  }
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

// Parse /proc/[pid]/stat
// Store the result in the argument pstat_data
void LinuxParser::ParseProcessStats(int pid, LinuxParser::ProcessStatData & pstat_data) {
  std::ifstream ifs = try_open(pid_directory(pid) + kStatFilename);
  std::string line;
  std::getline(ifs,line);
  
  // line is a space separated list of columns that we are interested in
  // But there is one column 'comm' which is a paren-enclosed filename, which can have spaces (and more parens)
  // We don't need any columns before that one, so we skip to the last ')' and avoid the problem
  int right_paren_idx{};
  try {
    right_paren_idx = line.find_last_of(')');
    line = line.substr(right_paren_idx+1);
  }
  catch(const std::out_of_range & error) {
    throw std::runtime_error(std::string("Error parsing ")+pid_directory(pid) + kStatFilename+": couldn't read past a right paren");
  }

  // It should be that we dropped 2 columns. Also, column numbers in "man proc" start counting at 1, not 0.
  constexpr int offset = (-2) + (-1);

  auto line_split = split_whitespace(line);
  // There are 52 columns and we dropped 2. There should be exactly 50 items in line_split,
  // unless the linux version is different from the one I'm developing on: newer linux -> more columns.
  // So we check only that there are enough columns for super old linux: 36

  if (line_split.size() < 36)  // Check !=50 for linux 3.5 till current (4.19), for better validation of the parse
    throw std::runtime_error(std::string("Error parsing ")+pid_directory(pid) + kStatFilename+": wrong number of columns");
  try {
    // The column numbers here are referenced in "man proc" under "/proc/[pid]/stat"
    pstat_data.utime = std::stoul(line_split[14+offset]);
    pstat_data.stime = std::stoul(line_split[15+offset]);
    pstat_data.cutime = std::stoul(line_split[16+offset]);
    pstat_data.cstime = std::stoul(line_split[17+offset]);
    pstat_data.starttime = std::stoull(line_split[22+offset]);
    pstat_data.vsize = std::stoul(line_split[23+offset]);
  }
  catch (const std::invalid_argument & error){
    throw std::runtime_error(std::string("Error parsing ")+pid_directory(pid) + kStatFilename+": unable to convert entries to numeric");
  }
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  std::ifstream ifs = try_open(pid_directory(pid) + kCmdlineFilename);
  std::ostringstream oss;
  char c;
  while (ifs.get(c)){
    if (c=='\0') continue; // There are often nulls that mess up the c_str needed later
    oss << c;
  }
  return oss.str();
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  std::ifstream ifs = try_open(pid_directory(pid) + kStatusFilename);
  std::string line;
  while (std::getline(ifs, line)) {
    auto split_line = split(line, ":");
    if (split_line.size() >= 2 && split_line[0] == "Uid"){
      auto split_rhs = split_whitespace(split_line[1]);
      if (!split_rhs.empty())
        return split_rhs[0];
    }
  }
  throw std::runtime_error(std::string("Error parsing ")+pid_directory(pid) + kStatusFilename);
}

// Read and return the user associated with a UID
string LinuxParser::User(const std::string & uid) {
  std::ifstream ifs = try_open(kPasswordPath);
  std::string line;
  while (std::getline(ifs, line)) {
    auto split_line = split(line, ":");
    if (split_line.size() >= 3 && split_line[2] == uid)
      return split_line[0];
  }
  throw std::runtime_error(std::string("Error parsing ")+kPasswordPath+" to extract user with UID "+uid);
}
