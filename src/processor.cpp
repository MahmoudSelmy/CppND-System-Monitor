#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{
    long total = LinuxParser::Jiffies();
    long idle = LinuxParser::IdleJiffies();
    float cpu_util_ratio = (total - idle) / total;
    return cpu_util_ratio;
}