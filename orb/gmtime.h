#ifndef	GMTIME_H
#define GMTIME_H

#include "time.h"

struct tm* gmtime(register const time_t *timer);

#endif