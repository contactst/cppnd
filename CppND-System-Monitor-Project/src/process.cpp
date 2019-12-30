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

// Return this process's ID
int Process::Pid() 
{ 
  return pid_; 
}

// Return this process's CPU utilization
float Process::CpuUtilization() 
{ 
  long upTime = 0;
  float totalTime = 0.0; 
  float time_in_seconds = 0.0; 
  float cpu_utilization = 0.0; 

  std::vector<std::string> cpuStats = LinuxParser::CpuUtilization(pid_); 

  startTime_ = std::stof(cpuStats[21]); 
  uTime_     = std::stof(cpuStats[13]); 
  sTime_     = std::stof(cpuStats[14]); 
  cuTime_    = std::stof(cpuStats[15]); 
  csTime_    = std::stof(cpuStats[16]); 

  upTime = LinuxParser::UpTime(); 
  totalTime = uTime_ + sTime_ + csTime_ + cuTime_;
  time_in_seconds = upTime - (startTime_/Hertz_); 
  cpu_utilization = (totalTime/Hertz_)/time_in_seconds; 

  return cpu_utilization; 
}

// Return the command that generated this process
string Process::Command() 
{ 
  return LinuxParser::Command(pid_);
}

// Return this process's memory utilization
string Process::Ram() 
{ 
  return LinuxParser::Ram(pid_); 
}

// Return the user (name) that generated this process
string Process::User() 
{ 
  return LinuxParser::User(pid_); 
}

// Return the age of this process (in seconds)
long int Process::UpTime() 
{ 
  long upTime = LinuxParser::UpTime(); 
  long time_in_seconds = upTime - (startTime_/Hertz_);

  return time_in_seconds;
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process& a)  
{ 
  return std::stof(Ram()) < std::stof(a.Ram()); 
}

// Overload the "greater than" comparison operator for Process objects
bool Process::operator>(Process& a)
{
  return std::stof(Ram()) > std::stof(a.Ram()); 
}