#include "unity.h"

#include "strptime.h"
#include "gmtime.h"
#include "timegm.h"

#include "Project.h"
#include "Projects.h"
#include <string.h>

void setUp(void)
{
}

void tearDown(void)
{
}

void testConstructor(void)
{
	Projects projects;
	TEST_ASSERT_EQUAL_INT(0, projects.count());
}

void testAdd(void)
{
	Projects projects;
	Project project;

	project.hash = 123;
	projects.add(project);
	TEST_ASSERT_EQUAL_INT(123, projects.all[0].hash);
}

void testAddDuplicate(void)
{
	Projects projects;
	Project project;
	projects.add(project);
	projects.add(project);

	TEST_ASSERT_EQUAL_INT(1, projects.count());
}

void testCount(void)
{
	Projects projects;
	projects.add(Project());
	TEST_ASSERT_EQUAL_INT(1, projects.count());
}

void testCountLimit(void)
{
	Projects projects;
	Project project;
	for (int i=0; i<MAX_NUMBER_PROJECTS; i++)
	{
		project = Project();
		project.hash = i;
		projects.add(project);
	}
	TEST_ASSERT_EQUAL_INT(MAX_NUMBER_PROJECTS, projects.count());
}

void testMostRecentBuild(void)
{
	Projects projects;

	Project project = Project();
	project.hash = 1;
	project.setLastBuildTimeUTC("2012-06-28T16:23:55Z");
	projects.add(project);

	Project project_recent = Project();
	project_recent.hash = 2;
	project_recent.setLastBuildTimeUTC("2012-06-29T01:23:55Z");
	projects.add(project_recent);

	project = Project();
	project.hash = 3;
	project.setLastBuildTimeUTC("2012-06-27T16:23:55Z");
	projects.add(project);

	project = Project();
	project.hash = 4;
	project.setLastBuildTimeUTC("2012-05-27T16:23:55Z");
	projects.add(project);

	project = Project();
	project.hash = 5;
	project.setLastBuildTimeUTC("2011-06-27T16:23:55Z");
	projects.add(project);

	TEST_ASSERT_EQUAL_INT(project_recent.lastBuildTimeUTC, projects.mostRecentBuild().lastBuildTimeUTC);
	TEST_ASSERT_EQUAL_INT(project_recent.hash, projects.mostRecentBuild().hash);
}

void testFindPresent(void)
{
	Projects projects;

	Project project = Project();
	project.setHash("Project One");
	projects.add(project);

	project = Project();
	project.setHash("Project Two");
	projects.add(project);

	TEST_ASSERT_EQUAL_INT(1, projects.find(project));
}

void testFindAbsent(void)
{
	Projects projects;

	Project project = Project();
	project.setHash("Project One");
	projects.add(project);

	project = Project();
	project.setHash("Project Two");

	TEST_ASSERT_EQUAL_INT(-1, projects.find(project));
}
