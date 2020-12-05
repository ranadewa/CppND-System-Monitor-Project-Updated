#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, std::string user, std::string command): pid_(pid), user_(user), command_(command)
                                                                , utilisation_(0)
{
     auto activeTime = LinuxParser::ActiveJiffies(pid_) / sysconf(_SC_CLK_TCK);
     auto proccessTime = LinuxParser::UpTime(pid_);

      utilisation_ =  activeTime / (proccessTime == 0 ? 1 : proccessTime);
}

int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { return utilisation_; }

string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { return CpuUtilization() < a.CpuUtilization(); }