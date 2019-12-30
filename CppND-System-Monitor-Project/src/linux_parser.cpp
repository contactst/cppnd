#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iomanip>

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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{ 
  float totalMemory = SearchForKey<int>("MemTotal:", kProcDirectory + kMeminfoFilename);
  float freeMemory  = SearchForKey<int>("MemFree:", kProcDirectory + kMeminfoFilename);

  return ((totalMemory - freeMemory) / totalMemory); 
}

// Read and return the system uptime
long LinuxParser::UpTime() 
{ 
  return SearchForValue<long>(kProcDirectory + kUptimeFilename);  
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization(int pid) 
{ 
  std::string lineData; 
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  std::getline(stream, lineData); 
  std::istringstream linestream(lineData); 
  std::istream_iterator<std::string> start(linestream), stop; 
  std::vector<std::string> cpuStats(start, stop);   
  return cpuStats; 
}

// Read and return Processor utilization
vector<float> LinuxParser::ProcessorUtilization()
{
  vector<float> curr_state; 
  float val = 0.0; 
  std::string lineData; 
  std::string keyData; 

  std::ifstream stream(kProcDirectory + kStatFilename); 
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

  return curr_state; 
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{ 
  return SearchForKey<int>("processes", kProcDirectory + kStatFilename);
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{ 
  return SearchForKey<int>("procs_running", kProcDirectory + kStatFilename);
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) 
{ 
  return SearchForValue<string>(kProcDirectory + to_string(pid) + kCmdlineFilename);
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) 
{
  std::stringstream memoryData; 
  float memoryDump = SearchForKey<float>("VmSize:", 
                     kProcDirectory + to_string(pid) + 
                     kStatusFilename); 
  memoryDump /= 1024; 
  memoryData << std::fixed << std::setprecision(1) << memoryDump; 
  return memoryData.str(); 
}

// Read and return the user associated with a process
string LinuxParser::User(int pid[[maybe_unused]]) 
{ 
  string lineData; 
  string data1;
  string data2; 
  int fileUserId;

  int userId = SearchForKey<int>("Uid:", kProcDirectory + to_string(pid)
                                 +kStatusFilename); 
  
  std::ifstream fstream(kPasswordPath); 

  if(fstream.is_open()){
    while(std::getline(fstream, lineData)){
      std::replace(lineData.begin(), lineData.end(), ':', ' '); 
      std::istringstream linestream(lineData); 
      while(linestream >> data1 >> data2 >> fileUserId){
        if(fileUserId == userId){
          return data1; 
        }
      }
    }
  }

  return data1; 
}

/*------- Methods not implemented ---------*/ 
// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }