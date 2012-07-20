#include "unity.h"

#include "strptime.h"
#include "gmtime.h"
#include "timegm.h"

#include "Project.h"

#include <string.h>

void setUp(void)
{
}

void tearDown(void)
{
}

void testConstructor(void)
{
	Project project = Project();
}

void testLastBuildTimeUTC(void)
{
	Project project;

	project.setLastBuildTimeUTC("2012-06-29T16:28:05Z");

	TEST_ASSERT_EQUAL_INT(1340987285, project.lastBuildTimeUTC);
}
