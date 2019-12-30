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
  std::string lineData, keyData; 
  std::vector<float> curr_state;
  float val = 0.0; 

  // Extract the information from /proc/state file and store in current state
  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename); 
  if(stream.is_open()){
    while(std::getline(stream, lineData)){
      std::istringstream lineStream(lineData);
      while(lineStream >> keyData){
        if("cpu" == keyData){
          while(lineStream >> val){
            curr_state.emplace_back(val); 
          }
        }
      }
    }
  }

  // Compute total usage, idle usage and non idle usage for current and previous state
  double currIdle    = curr_state[3] + curr_state[4]; 
  double prevIdle    = prev_state[3] + prev_state[4];      
  float currNonIdle = curr_state[0] + curr_state[1] + 
                      curr_state[2] + curr_state[5] + 
                      curr_state[6] + curr_state[7];
  float prevNonIdle = prev_state[0] + prev_state[1] + 
                      prev_state[2] + prev_state[5] + 
                      prev_state[6] + prev_state[7];
  float currTotal   = currIdle + currNonIdle; 
  float prevTotal   = prevIdle + prevNonIdle; 

  double CPU_Utilization = ((currTotal - prevTotal) - (currIdle - prevIdle))/(currTotal - prevTotal); 
    
  // Backup current usage statics as previous data for next computation. 
  for(unsigned size = 0; size < curr_state.size(); size++){
    prev_state[size] = curr_state[size]; 
  }

  return CPU_Utilization; 
}