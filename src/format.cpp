#include <string>
#include <chrono>

#include "format.h"

using std::string;
using namespace std::chrono;

string format(long value)
{
    return (value < 10) ? "0" + std::to_string(value) : std::to_string(value);
}

string Format::ElapsedTime(long seconds) {
    std::chrono::seconds sec(seconds);
    string hours =  format(duration_cast<std::chrono::hours>(sec).count());
    string minutes = format(duration_cast<std::chrono::minutes>(sec).count()  % 60);

    return hours + ":" + minutes + ":" + format(seconds % 60);
   }