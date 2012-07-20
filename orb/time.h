#ifndef TIME_H
#define TIME_H

typedef unsigned long time_t;

struct tm
{
  int tm_sec;                   // Seconds after the minute [0, 61]
  int tm_min;                   // Minutes after the hour [0, 59]
  int tm_hour;                  // Hours since midnight [0, 23]
  int tm_mday;                  // Day of the month [1, 31]
  int tm_mon;                   // Months since January [0, 11]
  int tm_year;                  // Years since 1900
  int tm_wday;                  // Days since Sunday [0, 6]
  int tm_yday;                  // Days since January 1 [0, 365]
  int tm_isdst;                 // Daylight Saving Time flag
};

#define YEAR0   	1900
#define	EPOCH_YEAR	1970
#define	SECSPERHOUR	3600
#define	SECSPERDAY	86400
#define	TM_YEAR_BASE	1900

#define 	SECS_DAY   (24L * 60L * 60L)
#define 	LEAPYEAR(year)   (!((year) % 4) && (((year) % 100) || !((year) % 400)))
#define 	YEARSIZE(year)   (LEAPYEAR(year) ? 366 : 365)

const unsigned int _ytab[2][12] = {
    { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
    { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

#endif