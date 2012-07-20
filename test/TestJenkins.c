#include "unity.h"

#include "strptime.h"
#include "gmtime.h"
#include "timegm.h"

#include "Project.h"
#include "Projects.h"
#include "XMLParser.h"

#include "Jenkins.h"
#include <string.h>

void setUp(void)
{
}

void tearDown(void)
{
}

void testParseOneProject(void)
{
	Jenkins jenkins = Jenkins();

	char input[300] = "<Projects>\n<Project/>\n</Projects>";

	for (unsigned int i=0; i<strlen(input); i++)
	{
		jenkins.parse(input[i]);
	}
	TEST_ASSERT_EQUAL_INT(1, jenkins.projects.count());
}

void testParseTenProjects(void)
{
	Jenkins jenkins = Jenkins();

	char input[300] = "<Projects>\n<Project name=\"1\"/>\n<Project name=\"2\"/>\n<Project name=\"3\"/>\n<Project name=\"4\"/>\n<Project name=\"5\"/>\n<Project name=\"6\"/>\n<Project name=\"7\"/>\n<Project name=\"8\"/>\n<Project name=\"9\"/>\n<Project name=\"10\"/>\n</Projects>";

	for (unsigned int i=0; i<strlen(input); i++)
	{
		jenkins.parse(input[i]);
	}
	TEST_ASSERT_EQUAL_INT(MAX_NUMBER_PROJECTS, jenkins.projects.count());
}

void testParseSuccess(void)
{
	Jenkins jenkins = Jenkins();

	char input[300] = "<Projects><Project lastBuildStatus=\"Success\"/></Projects>";

	for (unsigned int i=0; i<strlen(input); i++)
	{
		jenkins.parse(input[i]);
	}
	TEST_ASSERT_TRUE(jenkins.projects.all[0].success);
}

void testParseFailure(void)
{
	Jenkins jenkins = Jenkins();

	char input[300] = "<Projects><Project lastBuildStatus=\"Failure\"/></Projects>";

	for (unsigned int i=0; i<strlen(input); i++)
	{
		jenkins.parse(input[i]);
	}
	TEST_ASSERT_FALSE(jenkins.projects.all[0].success);
}

void testParseBuilding(void)
{
	Jenkins jenkins = Jenkins();

	char input[300] = "<Projects><Project activity=\"Building\"/></Projects>";

	for (unsigned int i=0; i<strlen(input); i++)
	{
		jenkins.parse(input[i]);
	}
	TEST_ASSERT_TRUE(jenkins.projects.all[0].building);
}


void testParseSleeping(void)
{
	Jenkins jenkins = Jenkins();

	char input[300] = "<Projects><Project activity=\"Sleeping\"/></Projects>";

	for (unsigned int i=0; i<strlen(input); i++)
	{
		jenkins.parse(input[i]);
	}
	TEST_ASSERT_FALSE(jenkins.projects.all[0].building);
}


void testParseAllAttributes(void)
{
	Jenkins jenkins = Jenkins();

	char input[300] = "<Projects>\n<Project name=\"Twenty - Master\" lastBuildLabel=\"61\" lastBuildTime=\"2012-06-28T16:28:00Z\" lastBuildStatus=\"Failure\" activity=\"Building\"/>\n</Projects>";

	for (unsigned int i=0; i<strlen(input); i++)
	{
		jenkins.parse(input[i]);
	}
	TEST_ASSERT_EQUAL_INT(1, jenkins.projects.count());
	TEST_ASSERT_EQUAL_INT(1340900880, jenkins.projects.all[0].lastBuildTimeUTC);
	TEST_ASSERT_EQUAL_INT(26996, jenkins.projects.all[0].hash);
	TEST_ASSERT_TRUE(jenkins.projects.all[0].building);
	TEST_ASSERT_FALSE(jenkins.projects.all[0].success);
}
