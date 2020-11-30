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
// Definition of the accessor functions of the class Process


// TODO: Return this process's ID
int Process::Pid() { return this-> pid_; /* Pointing to the process ID inside Process class */ }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    // total_time = utime + stime + cutime + cstime = ActiveJiffies(PID)
    long totaltime = LinuxParser::ActiveJiffies(this->pid_);
    // seconds = uptime - (starttime / Hertz) = UpTime(PID)
    long seconds = LinuxParser::UpTime(this->pid_);
    // cpu_usage = 100 * ((total_time / Hertz) / seconds)
    float cpuutilization = (((float)totaltime / sysconf(_SC_CLK_TCK)) / (float)seconds);
    this->cpuutilization_ = cpuutilization;
    return cpuutilization;
}

// TODO: Return the command that generated this process
string Process::Command() {
    string command= LinuxParser::Command(this->pid_);
    return command; 
}

// TODO: Return this process's memory utilization
string Process::Ram() {
    string ram = LinuxParser::Ram(this->pid_);
    return ram;
}

// TODO: Return the user (name) that generated this process
string Process::User() {
    string user = LinuxParser::User(this->pid_);
    return user;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    long int uptime = LinuxParser::UpTime(this->pid_);
    return uptime;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
// Purpose: sort the processes which use most memory, or cpu
bool Process::operator>(Process const& a) const {
    //taking the cpu usage as a parameter to be compared
   return a.cpuutilization_ > this->cpuutilization_;
}