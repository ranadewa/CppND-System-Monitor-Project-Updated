#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float utilisation = 0;

  std::ifstream stream(kProcDirectory + kMeminfoFilename);

  if(stream.is_open())
  {
    std::string line;
    getline(stream, line);
    
    std::string totalLabel, freeLabel, unit;
    float total, freeM;

    {
      std::istringstream lineStream(line);
      lineStream >> totalLabel >> total >> unit;
    }
    
    getline(stream, line);
    {
      std::istringstream lineStream(line);
      lineStream >> freeLabel >> freeM >> unit;
    }
    
    utilisation = (total - freeM) / total;
  }

  return utilisation;
  
 }

long LinuxParser::UpTime() {
  long uptime =0;

   std::ifstream stream(kProcDirectory + kUptimeFilename);
   
   if(stream.is_open())
   {
     std::string line;
     getline(stream, line);

     std::istringstream lineStream(line);
     double systemUpTime, idleTime;

     lineStream >> systemUpTime >> idleTime;

     uptime = (long) systemUpTime;
   }

   return uptime;
  }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

vector<string> LinuxParser::CpuUtilization() { 
  vector<string> utilization;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open())
  {
    string line;
    getline(stream, line);
    std::istringstream lineStream(line);

    string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;

    lineStream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;

    utilization = {user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice};
  }

  return utilization; }

string LinuxParser::getTag(std::regex const& re, int index) {
  string result;
  std::smatch sm;
  std::ifstream stream(kProcDirectory + kStatFilename);

  if(stream.is_open())
  {
    for (std::string line; getline(stream, line);)
    {
      if(std::regex_match(line, sm, re))
      {
        result = sm[index].str();
      }
    }
  }

  return result;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
    return std::stoi(getTag(totalProcessTag, 1));  
 }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
   return std::stoi(getTag(runningProcessTag, 1)); 
  }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 

    string command;

    std::ifstream stream(kProcDirectory+ std::to_string(pid)+kCmdlineFilename);

    if(stream.is_open())
    {
      getline(stream, command);
    }
    
    return command; 
  }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid)
 { 
  return getTag(uidTag, 1); 
  }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  long uptime = 0;


  return uptime; 
  }