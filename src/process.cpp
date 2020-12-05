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

Process::Process(int pid, std::string user, std::string command): pid_(pid), utilisation_(0), user_(user), command_(command)
{
 float activeTime = static_cast<float> (LinuxParser::ActiveJiffies(pid_)) / sysconf(_SC_CLK_TCK);
 float proccessTime = LinuxParser::UpTime(pid_);

      utilisation_ =  activeTime / (proccessTime == 0 ? 1 : proccessTime);
}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() const { return utilisation_; }

string Process::Command() { return command_; }

string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() { return user_; }

long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process const& a) const { return a.utilisation_ < utilisation_; }