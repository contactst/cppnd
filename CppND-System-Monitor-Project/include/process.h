#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <iterator>
#include <sstream>
#include <iostream>

#include "linux_parser.h"

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process& a);
  bool operator>(Process& a);

  Process(int pid, long Hertz):pid_(pid), Hertz_(Hertz)
  {

  }

 private:
  int  pid_; 
  long Hertz_; 
  float startTime_; 
  float uTime_;
  float sTime_;
  float cuTime_;
  float csTime_; 
};

#endif