#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

using std::vector;
using std::string; 

class System {
 public:
  Processor& Cpu();                   
  vector<Process>& Processes();  
  float MemoryUtilization();          
  long UpTime();                      
  int TotalProcesses();               
  int RunningProcesses();             
  string Kernel();               
  string OperatingSystem();      
  System(); 

 private:
  Processor cpu_ = {};
  vector<Process> processes_;
  vector<int> current_pids_; 
  long Hertz_; 

  void UpdateProcessList(); 
  void AddNewProcessToList(bool& procStatus); 
  void RemoveProcessFromList(bool& procStatus); 
};

#endif