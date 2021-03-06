#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

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
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
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
float ConvertLineToMemoryValue(string line)
{
  string memory_key, memory_value, kb_keyword;
  std::istringstream linestream(line);
  linestream >> memory_key >> memory_value >> kb_keyword;
  return stof(memory_value);
}

double ExstractValueFromLine(string line, int value_index)
{
  string value;
  std::istringstream linestream(line);
  while (value_index >= 0)
  {
    linestream >> value;
    value_index -= 1;
  }
  return stod(value);
}

float LinuxParser::MemoryUtilization() 
{
  float total_memory, free_memory;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) 
  {
    std::getline(stream, line);
    total_memory = ConvertLineToMemoryValue(line);
    std::getline(stream, line);
    free_memory = ExstractValueFromLine(line, 1);
    float memory_utilization_ratio = (total_memory - free_memory) / total_memory;
    return memory_utilization_ratio;
  }
  return 0.0;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() 
{
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) 
  {
    std::getline(stream, line);
    double up_time = ExstractValueFromLine(line, 1);
    return up_time;
  }
  return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies()
{ 
  auto cpu_values = LinuxParser::CpuUtilization();
  long sum = 0;

  for (auto& j : cpu_values) 
  {
    sum += stol(j);
  }
  return sum; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) 
{
  string line;
  string key;
  string value;
  long sum = 0;
  string process_path = kProcDirectory + to_string(pid) + kStatFilename;
  std::ifstream filestream(process_path);
  if (filestream.is_open()) 
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    int i = 0;
    while (linestream >> value)
    {
      i += 1;
      if (i == 14 || i == 15) 
      {
        sum += std::stol(value);
      }
      if (i == 15)
      {
        break;
      } 
    }
  }

  return sum;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies()
{ 
  auto cpu_values = LinuxParser::CpuUtilization();
  long active_jiffies = stol(cpu_values[kUser_]) + stol(cpu_values[kNice_]) + 
                        stol(cpu_values[kSystem_]) + stol(cpu_values[kIRQ_]) +
                        stol(cpu_values[kSoftIRQ_]) + stol(cpu_values[kSteal_]);
  return active_jiffies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{
  auto cpu_values = LinuxParser::CpuUtilization();
  long idle_jiffies = stol(cpu_values[kIdle_]) + stol(cpu_values[kIOwait_]); 
  return idle_jiffies; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{ 
  string value;
  string line;
  vector<string> values;
  std::ifstream file_stream(kProcDirectory + kStatFilename);
  if (file_stream.is_open()) 
  {
    std::getline(file_stream, line);
    std::istringstream line_stream(line);
    while (line_stream >> value) {
      if (value != "cpu") {
        values.push_back(value);
      }
    }
  }
  return values;  
}

int ExtractProcessesCount(string selected_key)
{
  string line;
  string key;
  string value;
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value) 
      {
        if (key == selected_key) 
        {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;
}
// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{
  return ExtractProcessesCount("processes");
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{ 
  return ExtractProcessesCount("procs_running"); 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{ 
  string line;
  string process_path = kProcDirectory + to_string(pid) + kCmdlineFilename;
  std::ifstream filestream(process_path);
  if (filestream.is_open()) 
  {
    std::getline(filestream, line);
    return line;
  }
  return string(); 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string ExtractProcessStatusAttribute(int pid, string attribute_key)
{
  string line;
  string key;
  string value;
  string process_path = LinuxParser::kProcDirectory + to_string(pid) +LinuxParser::kStatusFilename;
  std::ifstream filestream(process_path);
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value) 
      {
        if (key == attribute_key) 
        {
          return value;
        }
      }
    }
  }
  return string(); 
}
string LinuxParser::Ram(int pid) 
{ 
  return ExtractProcessStatusAttribute(pid, "VmSize:"); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) 
{ 
  return ExtractProcessStatusAttribute(pid, "Uid:");  
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) 
{ 
  string line;
  string username;
  string password;
  string uid;

  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> username >> password >> uid) 
      {
        if (uid == LinuxParser::Uid(pid)) 
        {
          return username;
        }
      }
    }
  }
  return string(); 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) 
{ 
  string line;
  long value;
  int starttime = 0;
  string process_path = kProcDirectory + to_string(pid) + kStatFilename;
  std::ifstream filestream(process_path);
  if (filestream.is_open()) 
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);

    value = ExstractValueFromLine(line, 21);

    starttime = value / sysconf(_SC_CLK_TCK);
  }

  return starttime; 
}