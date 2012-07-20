#include "Project.h"

#include "strptime.h"
#include "timegm.h"

#include <string.h>

Project::Project()
{	
	hash = 0;
	lastBuildTimeUTC = 0;
    success = false;
    building = false;
}

void Project::setHash(const char* name)
{
	hash = computeHash(name);
}

unsigned int Project::computeHash(const char* s)
{
    unsigned int hash = 0;
    int c;
 
    while((c = *s++))
    {
        /* hash = hash * 33 ^ c */
        hash = ((hash << 5) + hash) ^ c;
    } 
    return hash;
}

void Project::setLastBuildTimeUTC(const char* s)
{
	lastBuildTimeUTC = computeTimeUTC(s);
}

time_t Project::computeTimeUTC(const char* s)
{
	// 2012-06-29T16:28:00Z
	strptime(s, "%Y-%m-%dT%H:%M:%S%z", &lastBuildTime);
	return timegm(&lastBuildTime);
}