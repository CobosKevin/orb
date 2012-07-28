#include "Jenkins.h"

#include <string.h>

#define SERVER_PATH "/cc.xml"

/*
    Handle the various build statuses returned by Jenkins server
*/
Jenkins::Jenkins()
{
    initialize();
}

void Jenkins::initialize()
{
    projects = Projects();
    currentProject = Project();
    xmlParser = XMLParser();
    xmlParser.setCallback(Jenkins::parserCallback, this);
}

/*
	Delegate parsing to xml parser
*/
void Jenkins::parse(char c)
{
    xmlParser.parse(c);
}

/*
	Handle tags to create project objects.
*/
void Jenkins::handleTag(bool fTagClosed, char* tag, char* attribute, char* value)
{
    if (strcmp(tag, "Project") == 0)
    {
        if (fTagClosed)
        {
            projects.add(currentProject);
            currentProject = Project();
        }
        else
        {
            // Assign relevant params
            handleAttributes(currentProject, attribute, value);
        }
    }
}

/*
	Parse tag attributes and assign properties in project object
*/
void Jenkins::handleAttributes(Project &project, char* attribute, char* value)
{
    if (strcmp(attribute, "name") == 0)
    {
        project.setHash(value);
    }
    else if (strcmp(attribute, "lastBuildTime") == 0)
    {
        project.setLastBuildTimeUTC(value);
    }
    else if (strcmp(attribute, "lastBuildStatus") == 0)
    {
        project.success = (strcmp(value, "Success") == 0);

    }
    else if (strcmp(attribute, "activity") == 0)
    {
        project.building = (strcmp(value, "Building") == 0);
    }
}

/*
	Static class method to handle parser callback
*/
void Jenkins::parserCallback(void* context, bool fTagClosed, char* tag, char* attribute, char* value)
{
    if (context)
    {
        Jenkins *jenkins = (Jenkins *)context;
        jenkins->handleTag(fTagClosed, tag, attribute, value);
    }
}

const char* Jenkins::getPath()
{
    return SERVER_PATH;
}