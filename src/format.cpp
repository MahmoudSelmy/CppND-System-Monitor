#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) 
{
    long hours = seconds / 3600;
    long minutes = seconds % 3600 / 60;
    long secs = seconds % 60;
    
    string hours_part =
      hours > 9 ? std::to_string(hours) : "0" + std::to_string(hours);

    string minutes_part = minutes > 9 ? ":" + std::to_string(minutes)
                                   : ":0" + std::to_string(minutes);
    
    string secs_part =
      secs > 9 ? ":" + std::to_string(secs) : ":0" + std::to_string(secs);

    string new_time = hours_part + minutes_part + secs_part;
    return new_time; 
}