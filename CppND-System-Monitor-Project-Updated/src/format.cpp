#include <string>
#include <cstdlib>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    div_t h, m;
    string format, format_hh, format_mm, format_ss;
    int hh, mm, ss;
    h = div(seconds,3600);
    hh = h.quot;
    if (hh < 10){
        format_hh = "0" + to_string(hh);
    }
    else
    {
        format_hh = to_string(hh);
    }

    m = div((seconds - (hh*3600)), 60);
    mm = m.quot;
    if (mm < 10){
        format_mm = "0" + to_string(mm);
    }
    else
    {
        format_mm = to_string(mm);
    }
    ss = seconds - (hh*3600) - (mm*60);
    if (ss < 10){
        format_ss = "0" + to_string(ss);
    }
    else
    {
        format_ss = to_string(ss);
    }
    format = format_hh + ":" + format_mm + ":" + format_ss;
    return format;
}