#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stoi;
using std::stol;
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
  string os;
  string kernel;
  string version;
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
  string line_1;
  string line_2;
  string key_1;
  string key_2;
  float MemTotal;
  float MemFree;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line_1);   
    std::istringstream linestream_1(line_1);
    linestream_1 >> key_1 >> MemTotal;

    std::getline(filestream, line_2);
    std::istringstream linestream_2(line_2);
    linestream_2 >> key_2 >> MemFree;
  }
  return (MemTotal - MemFree) / MemTotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime;
  string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  long jiffies = 0;
  vector<string> cpu = LinuxParser::CpuUtilization();
  for(int i = 0; i < 10; i++){ 
    if(cpu[i]!=" "){
      jiffies += std::stol(cpu[i]);
    }
  }
  return jiffies; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  long activejiffies = 0;
  char c;
  string field;
  std::vector<string> fields{};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if(filestream.is_open()){
    while(filestream.get(c)){
      field += c;
      if(c == '\n'){
        fields.push_back(field);
        field.clear();
        break;
      }
      if(c == ' '){
        fields.push_back(field);
        field.clear();     
      }
    }
  }
  if(fields[13] != " " && fields[14] != " " && fields[15] != " " && fields[16] != " "){
    activejiffies = std::stol(fields[13] + fields[14] + fields[15] + fields[16]);
  }
  return activejiffies;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  // Active time = sum of all cpu fields, excluding 'idle' + 'iowait' 
  vector<string> cpu = LinuxParser::CpuUtilization();
  long activejiffies = 0;
  for(int i = 0; i < 10; i++){
    if(i != 3 && i != 4){ // excluding 'idle' + 'iowait' fields
      if(cpu[i] != " "){
        activejiffies += std::stol(cpu[i]);
      }
    }
  }
  return activejiffies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  long idlejiffies = 0;
  // Idle time = 'idle' + 'iowait' 
  vector<string> cpu = LinuxParser::CpuUtilization();
  if(cpu[3] != " " && cpu [4] != " "){
    idlejiffies = std::stol(cpu[3]) + std::stol(cpu[4]);
  }
  return idlejiffies;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string str;
  char c;
  vector<string> states{};
  std::ifstream filestream (kProcDirectory + kStatFilename);
  if (filestream.is_open()){
    while(filestream.get(c)){
      str += c;
      if(c == '\n'){
        states.push_back(str);
        str.clear();
        break;
      }
      if(c == ' '){
        states.push_back(str);
        str.clear();
      }
    }
    states.erase(states.begin()); //delete the first member 'cpu'
  }
  return states;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  int totalprocesses;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line); 
      while(linestream >> key >> totalprocesses){
        if (key == "processes") return totalprocesses;
      }
    }
  }
  return totalprocesses;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  int runningprocesses;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line); 
      while(linestream >> key >> runningprocesses){
        if (key == "procs_running") return runningprocesses;
      }
    }
  }
  return runningprocesses;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(filestream.is_open()){
    std::getline(filestream, line);
  }  
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line, key, ram;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(filestream.is_open()){
    while(getline(filestream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key;
      if(key == "VmSize") linestream >> ram;
    }
  }
  return to_string(stol(ram) / 1024);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line, key, uid;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> uid){
        if(key == "Uid:") return uid;
      }
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line, user, pwd, uid;
  string uidpid = LinuxParser::Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> user >> pwd >> uid){
        if(uid == uidpid){
          return user;
        }
      }
    }
  }
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  char c;
  long starttime = 0;
  long uptime;
  string str;
  vector<string> states{};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if(filestream.is_open()){
    while(filestream.get(c)){
      str += c;
      if(c == '\n'){
        states.push_back(str);
        str.clear();
        break;
      }
      if(c == ' '){
        states.push_back(str);
        str.clear();
      }
    }
  }
  if(states[21] != " "){
    starttime = std::stol(states[21]) / sysconf(_SC_CLK_TCK);
  }
  
  return uptime = LinuxParser::UpTime() - starttime;
}