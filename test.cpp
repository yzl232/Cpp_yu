#include <iostream>
#include <ctime>
using namespace std;

int main()
{
   std::tm time_in = { 0, 0, 0, // second, minute, hour
        4, 9, 1984 - 1900 }; // 1-based day, 0-based month, year since 1900

std::time_t time_temp = std::mktime( & time_in );

// the return value from localtime is a static global - do not call
// this function from more than one thread!
std::tm const *time_out = std::localtime( & time_temp );

std::cout << "I was born on (Sunday = 0) D.O.W. " << time_out->tm_wday << '\n';

   return 0;
}
