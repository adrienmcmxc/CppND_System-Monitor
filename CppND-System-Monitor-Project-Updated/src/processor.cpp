#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    long activejiffies = LinuxParser::ActiveJiffies();
    long totaljiffies = LinuxParser::Jiffies();
    this->cpuutilization_ = (float)activejiffies / (float)totaljiffies;
    return cpuutilization_;
}