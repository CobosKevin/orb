#ifndef PROJECTS_H
#define PROJECTS_H

#include "ArduinoUno.h"
#include "Project.h"

#define MAX_NUMBER_PROJECTS 10

class Projects
{
public:
    Projects();

    void add(Project project);
    int count();
    void clear();
    Project all[MAX_NUMBER_PROJECTS];
    Project mostRecentBuild();
    int find(Project project);
    int building();
    bool hasFailure();

private:
    byte numberProjects;
};

#endif
