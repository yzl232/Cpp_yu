#include <iostream>
#include <ctime>
using namespace std;

int main()
{
    //char* weekdays[] =  { "Sun", "Mon", "Tues", "Wednes", "Thurs", "Fri", "Satur" };
    string weekdays[7] = { "Sun", "Mon", "Tues", "Wednes", "Thurs", "Fri",
    			"Satur" };
    string weekDay;
    int weekDayNum;
	int day = 25;
	int month = 4;
	int year = 2014;
    std::tm time_in = { 0, 0, 0, // second, minute, hour
         day-0, month-1, year - 1900 }; // 1-based day, 0-based month, year since 1900
   
std::time_t time_temp = std::mktime( & time_in );

// the return value from localtime is a static global - do not call
// this function from more than one thread!
std::tm const *time_out = std::localtime( & time_temp );

//std::cout << "I was born on (Sunday = 0) D.O.W. " << time_out->tm_wday << '\n';
   weekDayNum = time_out->tm_wday;
   weekDay =  weekdays[weekDayNum] ;
   std::cout << weekDay <<"\n";
   
   
   
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
