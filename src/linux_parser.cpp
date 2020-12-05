#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <iostream>
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
long LinuxParser::ActiveJiffies(int pid) { 
  
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);

  long utime = 0, stime = 0, cutime = 0, cstime =0;

    if(stream.is_open())
    {
      string line;
      getline(stream, line);

      std::istringstream lineStream(line);

      for (int i=0;i<14;i++){

        lineStream >>utime;

      }

      lineStream >> stime >> cutime >> cstime; // 15, 16, 17
    }

    return utime + stime + cutime + cstime; 
  }

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

string getTag(std::string const& path, std::regex const& re, int index) {
  string result{"0"};
  std::smatch sm;
  std::ifstream stream(path);

  if(stream.is_open())
  {
    for (std::string line; getline(stream, line);)
    {
      if(std::regex_match(line, sm, re))
      {
        result = sm[index].str();
        break;
      }
    }
  }

  return result;
}

int LinuxParser::TotalProcesses() { 
    return std::stoi(getTag(kProcDirectory + kStatFilename, totalProcessTag, 1));  
 }

int LinuxParser::RunningProcesses() { 
   return std::stoi(getTag(kProcDirectory + kStatFilename, runningProcessTag, 1)); 
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
string LinuxParser::Ram(int pid) { 

  int megaByteValue = std::stoi(getTag(kProcDirectory + std::to_string(pid)+ kStatusFilename, memoryTag, 1)) / 1000;

  return std::to_string(megaByteValue);
 }

string LinuxParser::Uid(int pid)
 { 
  return getTag(kProcDirectory + std::to_string(pid) + kStatusFilename, uidTag, 1); 
 }

string LinuxParser::User(int pid) { 
  string user;

  string tag(R"((\w+):x:)");
  auto uid = Uid(pid);
  tag += uid + string(".*");

  const std::regex userTag(tag);

  user = getTag(kPasswordPath, userTag, 1);

  return user; 
  }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 

  long uptime = 0;

  try
  {
    auto jiffies = std::stol(getTag(kProcDirectory + std::to_string(pid) + kStatFilename, processStartTimeTag, 2));
    auto systemUptime = LinuxParser::UpTime();
    uptime = systemUptime - jiffies / sysconf(_SC_CLK_TCK);
  }
  catch(const std::exception& e)
  {
    std::cerr << "Error in parsing uptime for pid: " << pid << " " << e.what() << '\n';
  }
    

    return uptime;
  }