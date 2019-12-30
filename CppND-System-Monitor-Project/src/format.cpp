#include <string>
#include <chrono>
#include <sstream> 
#include "format.h"
using std::string;
using namespace std::chrono; 

/*!
   Description : Helper function to convert time in seconds to HH-MM-SS format.  

   Detail      : Using std chrono library, we detect the duration cast from 
                 seconds to minutes and hours, store them in a stringstream
                 to push back to the caller function. 

   Parameters  : seconds - value of type 'long'

   Return      : output  - value of type 'string'. Data is encompassed in a 
                 format of hours-minutes-seconds
 */ 
string Format::ElapsedTime(long times) 
{   
    std::chrono::seconds secs{times}; 

    auto mins = duration_cast<minutes>(secs);
    secs     -= duration_cast<seconds>(mins);  
    auto hour = duration_cast<hours>(mins); 
    mins     -= duration_cast<minutes>(hour); 

    std::stringstream output; 
    output << hour.count() << ":" << mins.count() << ":" << secs.count(); 

    return output.str(); 
}