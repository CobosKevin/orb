#ifndef PROJECTS_H
#define PROJECTS_H

#include "Project.h"

#define MAX_NUMBER_PROJECTS 8

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
	    int numberProjects;
};

#endif
