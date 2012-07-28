#include "Projects.h"

#include <string.h>

Projects::Projects()
{
    numberProjects = 0;
}

/*
	Add project to the array.
	If project already exists, replace it instead.
*/
void Projects::add(Project project)
{
    if (numberProjects < MAX_NUMBER_PROJECTS)
    {
        int index;

        // Already exist?
        if ((index = find(project)) >= 0)
        {
            all[index] = project;
        }
        else
        {
            all[(unsigned int)numberProjects] = project;
            numberProjects++;
        }
    }
}

int Projects::count()
{
    return numberProjects;
}

void Projects::clear()
{
    numberProjects = 0;
}

Project Projects::mostRecentBuild()
{
    int mostRecentIndex = 0;

    for (int i=0; i < numberProjects; i++)
    {
        if (all[i].lastBuildTimeUTC > all[mostRecentIndex].lastBuildTimeUTC)
        {
            mostRecentIndex = i;
        }
    }

    return all[mostRecentIndex];
}

/*
	Return the index of the first project that's not sleeping. -1 otherwise.
*/
int Projects::building()
{
    for (int i=0; i < numberProjects; i++)
    {
        if (all[i].building)
        {
            return i;
        }
    }

    return -1;
}

/*
	Look for project matching in array and return index if found or -1 if not present.
*/
int Projects::find(Project project)
{

    for (int i=0; i < numberProjects; i++)
    {
        if (all[i].hash == project.hash)
        {
            return i;
        }
    }

    return -1;
}

/*
	Return true if any project has failed.
*/
bool Projects::hasFailure()
{
    for (int i=0; i < numberProjects; i++)
    {
        if (!all[i].success)
        {
            return true;
        }
    }

    return false;
}
