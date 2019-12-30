#include "processor.h"
#include "linux_parser.h"

#include <iostream> 

/*!
   Description : Helper function to compute CPU utilization percentage.   

   Detail      : Process the CPU usage information from /proc/stat and parse
                 the information into idle and non-idle parameters. CPU utilization
                 will be computed by keeping track of previous usage information,
                 current idle stats and overall delta of previous and current 
                 CPU usage statistics. 

   Return      : CPU_Utilization - value of type 'float'. 
 */ 
float Processor::Utilization() 
{ 
  // Extract CPU utilization information from /proc/stat 
  std::vector<float> curr_state = LinuxParser::ProcessorUtilization();

  // Compute total usage, idle usage and non idle usage for current and previous state
  double currIdle    = curr_state[pIdle] + curr_state[pIowait]; 
  double prevIdle    = prev_state[pIdle] + prev_state[pIowait];      
  float currNonIdle = curr_state[pUser] + curr_state[pNice] + 
                      curr_state[pSystem] + curr_state[pIrq] + 
                      curr_state[pSoftirq] + curr_state[pSteal];
  float prevNonIdle = prev_state[pUser] + prev_state[pNice] + 
                      prev_state[pSystem] + prev_state[pIrq] + 
                      prev_state[pSoftirq] + prev_state[pSteal];
  float currTotal   = currIdle + currNonIdle; 
  float prevTotal   = prevIdle + prevNonIdle; 

  double CPU_Utilization = ((currTotal - prevTotal) - (currIdle - prevIdle))/(currTotal - prevTotal); 
    
  // Backup current usage statics as previous data for next computation. 
  for(unsigned size = 0; size < curr_state.size(); size++){
    prev_state[size] = curr_state[size]; 
  }

  return CPU_Utilization; 
}