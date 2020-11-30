#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() {
    return this->cpu_; 
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    // Defining a list of PIDs from LinuxParser
    vector<int> pids = LinuxParser::Pids();
    // Dumping the elements of PID list into a container of the type Process
    for (auto pid : pids){
        processes_.push_back(Process(pid));
    }
    // Sorting the list of processes in descending PID
    std::sort(processes_.begin(), processes_.end(), [](Process const& a, Process const& b){
        return a > b;
    });
    return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() {
    std::string system = LinuxParser::Kernel();
    return system;
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() {
    float memory = LinuxParser::MemoryUtilization();
    return memory;
}

// TODO: Return the operating system name
std::string System::OperatingSystem() {
    std::string os = LinuxParser::OperatingSystem();
    return os;
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() {
    int runningprocs = LinuxParser::RunningProcesses();
    return runningprocs;
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() {
    int totalprocs = LinuxParser::TotalProcesses();
    return totalprocs;
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() {
    int uptime = LinuxParser::UpTime();
    return uptime;
}