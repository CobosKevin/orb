#ifndef PROJECT_H
#define PROJECT_H

#include "time.h"

class Project
{
	public:
	    Project();
		void setHash(const char* name);

	    unsigned int hash;
	    bool success;
	    bool building;
        time_t lastBuildTimeUTC;
        void setLastBuildTimeUTC(const char* s);

	private:
		unsigned int computeHash(const char* s);
        time_t computeTimeUTC(const char* s);
        struct tm lastBuildTime;
};

#endif
