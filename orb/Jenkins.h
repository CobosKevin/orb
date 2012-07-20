#ifndef JENKINS_H
#define JENKINS_H

#include "Project.h"
#include "Projects.h"
#include "XMLParser.h"

#define MAX_HOST_LENGTH 30

class Jenkins
{
	public:
    	Jenkins();
    	Jenkins(const char* host);

	    void parse(char c);

		static void parserCallback(void* context, bool fTagClosed, char* tag, char* attribute, char* value);

		void handleTag(bool fTagClosed, char* tag, char* attribute, char* value);

		Projects projects;

		void setHost(const char* host);
		char* getHost();
		const char* getPath();

	private:
		void initialize();
		void handleAttributes(Project &project, char* attribute, char* value);
		XMLParser xmlParser;
		Project currentProject;
		char host[MAX_HOST_LENGTH+1];
};

#endif