#ifndef JENKINS_H
#define JENKINS_H

#include "Project.h"
#include "Projects.h"
#include "XMLParser.h"

class Jenkins
{
public:
    Jenkins();

    void parse(char c);

    static void parserCallback(void* context, bool fTagClosed, char* tag, char* attribute, char* value);

    void handleTag(bool fTagClosed, char* tag, char* attribute, char* value);

    Projects projects;

    const char* getPath();

private:
    void initialize();
    void handleAttributes(Project &project, char* attribute, char* value);
    XMLParser xmlParser;
    Project currentProject;
};

#endif