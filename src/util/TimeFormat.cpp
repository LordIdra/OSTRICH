#include "TimeFormat.h"



auto TimeFormat::FormatTime(const int rawSeconds) -> string {
    const int seconds = div(rawSeconds, 60).rem;
    const int secondsQuotient = div(rawSeconds, 60).quot;
    const int minutes = div(secondsQuotient, 60).rem;
    const int minutesQuotient = div(secondsQuotient, 60).quot;
    const int hours = div(minutesQuotient, 24).rem;
    const int hoursQuotient = div(minutesQuotient, 24).quot;
    const int days = div(hoursQuotient, 365).rem;
    const int years = div(hoursQuotient, 365).quot;
    return std::to_string(years)   + "y " + 
           std::to_string(days)    + "d " + 
           std::to_string(hours)   + "h " +
           std::to_string(minutes) + "m " +
           std::to_string(seconds) + "s";
}
