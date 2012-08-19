#ifndef PROJECT_H
#define PROJECT_H

#include "time.h"

class Project
{
public:
    Project();
    void setHash(const char* name);
    void setLastBuildTimeUTC(const char* s);
    bool valid();

    unsigned int hash;
    bool success;
    bool building;
    time_t lastBuildTimeUTC;

private:
    unsigned int computeHash(const char* s);
    time_t computeTimeUTC(const char* s);
};

#endif
