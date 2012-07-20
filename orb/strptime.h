#ifndef	STRPTIME_H
#define STRPTIME_H

#include "time.h"

char* strptime( char const *buf, char const *fmt, struct tm *tm);

#endif