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
int Process::Pid() 
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
    float ratio = process_jiffies / total_jiffies;
    return ratio; 
}

// TODO: Return the command that generated this process
string Process::Command() 
{ 
    return string(); 
}

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

// TODO: Return the user (name) that generated this process
string Process::User() { return string(); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }