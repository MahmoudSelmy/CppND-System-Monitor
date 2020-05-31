#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{
    long total = LinuxParser::Jiffies();
    if(total == 0)
    {
        return 0.0;
    }
    long active = LinuxParser::ActiveJiffies();
    float cpu_util_ratio = (active * 1.0) / total;
    return cpu_util_ratio;
}