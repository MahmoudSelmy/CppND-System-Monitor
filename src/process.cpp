#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int process_id):process_id(process_id)
{

}
// TODO: Return this process's ID
int Process::Pid() const
{ 
    return process_id; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() 
{
    long process_jiffies = LinuxParser::ActiveJiffies(process_id);
    long total_jiffies = LinuxParser::ActiveJiffies();
    if(total_jiffies == 0)
    {
        return 0.0;
    }
    float ratio = (process_jiffies*1.0) / total_jiffies;
    return ratio; 
}

// TODO: Return the command that generated this process
string Process::Command() 
{ 
    return LinuxParser::Command(process_id); 
}

// TODO: Return this process's memory utilization
string Process::Ram() 
{ 
    return LinuxParser::Ram(process_id); 
}

// TODO: Return the user (name) that generated this process
string Process::User() 
{ 
    return LinuxParser::User(process_id); 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime()
{ 
    return LinuxParser::UpTime(process_id); 
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const 
{ 
    int a_process_id = a.Pid();
    return LinuxParser::ActiveJiffies(process_id) > LinuxParser::ActiveJiffies(a_process_id); 
}