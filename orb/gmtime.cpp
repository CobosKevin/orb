#include "gmtime.h"

/*
    Converts unix time into time structure
*/
struct tm * gmtime(const time_t *timer)
{
    static struct tm br_time;
    struct tm *timep = &br_time;
    time_t time = *timer;
    unsigned long dayclock, dayno;
    int year = EPOCH_YEAR;

    dayclock = (unsigned long)time % SECS_DAY;
    dayno = (unsigned long)time / SECS_DAY;

    timep->tm_sec = dayclock % 60;
    timep->tm_min = (dayclock % 3600) / 60;
    timep->tm_hour = dayclock / 3600;
    timep->tm_wday = (dayno + 4) % 7;       /* day 0 was a thursday */
    while (dayno >= YEARSIZE(year))
    {
        dayno -= YEARSIZE(year);
        year++;
    }
    timep->tm_year = year - YEAR0;
    timep->tm_yday = dayno;
    timep->tm_mon = 0;
    while (dayno >= (unsigned long)_ytab[LEAPYEAR(year)][timep->tm_mon])
    {
        dayno -= _ytab[LEAPYEAR(year)][timep->tm_mon];
        timep->tm_mon++;
    }
    timep->tm_mday = dayno + 1;
    timep->tm_isdst = 0;

    return timep;
}