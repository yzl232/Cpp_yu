/*
 * date_to_weekday_worked.cpp
 *
 *  Created on: Apr 25, 2014
 *      Author: zhenglinyu
 */

#include <iostream>
#include <ctime>
using namespace std;

int main()
{
   std::tm time_in = { 0, 0, 0, // second, minute, hour
        25-0, 4-1, 2014 - 1900 }; // 1-based day, 0-based month, year since 1900

std::time_t time_temp = std::mktime( & time_in );

// the return value from localtime is a static global - do not call
// this function from more than one thread!
std::tm const *time_out = std::localtime( & time_temp );

std::cout << "I was born on (Sunday = 0) D.O.W. " << time_out->tm_wday << '\n';

   return 0;
}

/*

int main(void)
{
    time_str.tm_year = 2001 - 1900;
    time_str.tm_mon = 7 - 1;
    time_str.tm_mday = 4;
    time_str.tm_hour = 0;
    time_str.tm_min = 0;
    time_str.tm_sec = 1;
    time_str.tm_isdst = -1;
    if (mktime(&time_str) == -1)
        (void)puts("-unknown-");
    else {
        (void)strftime(daybuf, sizeof(daybuf), "%A", &time_str);
        (void)puts(daybuf);
    }
    return 0;



*/
