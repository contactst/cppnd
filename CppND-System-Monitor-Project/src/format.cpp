#include <string>
#include "format.h"
using std::string;

/*!
   Description : Helper function to convert time in seconds to HH-MM-SS format.  

   Detail      : Divide the time in seconds by 60 to get minutes, then divide 
                 minutes by 60 to get hours. Now wraparound the minutes and 
                 seconds to the range of 60 by doing a % 60. 

   Parameters  : seconds - value of type 'long'

   Return      : output  - value of type 'string'. Data is encompassed in a 
                 format of hours-minutes-seconds
 */ 
string Format::ElapsedTime(long seconds) 
{ 
    // Initialize all variables to default values
    long mins = 0;
    long hrs  = 0; 
    long secs = 0; 

    // Convert from seconds to minutes to hours and wraparound
    mins = seconds / 60; 
    hrs  = mins / 60; 
    secs = int(seconds % 60); 
    mins = int(mins % 60); 

    // Join all the integer values and convert to string format
    std::string output = std::to_string(hrs) + ":" + std::to_string(mins) + ":" + std::to_string(secs);

    return output; 
}