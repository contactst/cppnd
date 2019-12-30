#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
 
bool SortByRAM(Process& A, Process& B)
{
  return (A.Ram() > B.Ram()); 
} 

System::System()
{
  Hertz_ = sysconf(_SC_CLK_TCK); 
  UpdateProcessList(); 
}

void System::UpdateProcessList()
{
  bool proc_status_change = false; 
  current_pids_ = LinuxParser::Pids(); 
  AddNewProcessToList(proc_status_change); 
  RemoveProcessFromList(proc_status_change); 

  if(proc_status_change){
      std::sort(processes_.begin(), processes_.end(), SortByRAM); 
  }
}

void System::AddNewProcessToList(bool& procStatus)
{
  for(size_t index = 0; index < current_pids_.size(); index++){
    int pid = current_pids_[index];
       
    if(std::find_if(processes_.begin(), processes_.end(),
      [pid](Process& newProc){return newProc.Pid() == pid;}) == processes_.end()){
      procStatus = true; 
      Process process(current_pids_[index], Hertz_); 
      processes_.push_back(process); 
    }
  }
}

void System::RemoveProcessFromList(bool& procStatus)
{
  for(size_t index = 0; index < processes_.size(); index++){
    int pid = processes_[index].Pid(); 

    if(std::find(current_pids_.begin(), current_pids_.end(), pid) == current_pids_.end()){
       procStatus = true; 
       processes_.erase(processes_.begin() + index); 
    }
  }
}

// Return the system's CPU
Processor& System::Cpu() 
{ 
    return cpu_; 
}

// Return a container composed of the system's processes
vector<Process>& System::Processes() 
{ 
    return processes_; 
}

// Return the system's kernel identifier (string)
std::string System::Kernel() 
{ 
    return string(LinuxParser::Kernel()); 
}

// Return the system's memory utilization
float System::MemoryUtilization() 
{ 
    return LinuxParser::MemoryUtilization(); 
}

// Return the operating system name
std::string System::OperatingSystem() 
{ 
    return LinuxParser::OperatingSystem(); 
}

// Return the number of processes actively running on the system
int System::RunningProcesses() 
{ 
    return LinuxParser::RunningProcesses(); 
}

// Return the total number of processes on the system
int System::TotalProcesses() 
{ 
    return LinuxParser::TotalProcesses(); 
}

// Return the number of seconds since the system started running
long int System::UpTime() 
{ 
    return LinuxParser::UpTime(); 
}