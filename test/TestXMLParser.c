#include "unity.h"

#include "XMLParser.h"

#include <string.h>

void setUp(void)
{
}

void tearDown(void)
{
}

void callbackProjectsOpen(void* context, bool fTagClosed, char* tag, char* attribute, char* value)
{
	TEST_ASSERT_FALSE(fTagClosed);
	TEST_ASSERT_EQUAL_STRING("Projects", tag);
	TEST_ASSERT_EQUAL_STRING("", attribute);
	TEST_ASSERT_EQUAL_STRING("", value);
}

void callbackProjectOpen(void* context, bool fTagClosed, char* tag, char* attribute, char* value)
{
	TEST_ASSERT_FALSE(fTagClosed);
	TEST_ASSERT_EQUAL_STRING("Project", tag);
	TEST_ASSERT_EQUAL_STRING("", attribute);
	TEST_ASSERT_EQUAL_STRING("", value);
}

void callbackWebUrl(void* context, bool fTagClosed, char* tag, char* attribute, char* value)
{
	TEST_ASSERT_FALSE(fTagClosed);
	TEST_ASSERT_EQUAL_STRING("Project", tag);
	TEST_ASSERT_EQUAL_STRING("webUrl", attribute);
	TEST_ASSERT_EQUAL_STRING("http://ci.test.com/job/Job%20-%20One/", value);
}

void callbackName(void* context, bool fTagClosed, char* tag, char* attribute, char* value)
{
	TEST_ASSERT_FALSE(fTagClosed);
	TEST_ASSERT_EQUAL_STRING("Project", tag);
	TEST_ASSERT_EQUAL_STRING("name", attribute);
	TEST_ASSERT_EQUAL_STRING("Job - One", value);
}

void callbacLastBuildLabel(void* context, bool fTagClosed, char* tag, char* attribute, char* value)
{
	TEST_ASSERT_FALSE(fTagClosed);
	TEST_ASSERT_EQUAL_STRING("Project", tag);
	TEST_ASSERT_EQUAL_STRING("lastBuildLabel", attribute);
	TEST_ASSERT_EQUAL_STRING("61", value);
}

void callbackLastBuildTime(void* context, bool fTagClosed, char* tag, char* attribute, char* value)
{
	TEST_ASSERT_FALSE(fTagClosed);
	TEST_ASSERT_EQUAL_STRING("Project", tag);
	TEST_ASSERT_EQUAL_STRING("lastBuildTime", attribute);
	TEST_ASSERT_EQUAL_STRING("2012-06-28T16:28:00Z", value);
}

void callbackLastBuildStatus(void* context, bool fTagClosed, char* tag, char* attribute, char* value)
{
	TEST_ASSERT_FALSE(fTagClosed);
	TEST_ASSERT_EQUAL_STRING("Project", tag);
	TEST_ASSERT_EQUAL_STRING("lastBuildStatus", attribute);
	TEST_ASSERT_EQUAL_STRING("Success", value);
}

void callbackActivity(void* context, bool fTagClosed, char* tag, char* attribute, char* value)
{
	TEST_ASSERT_FALSE(fTagClosed);
	TEST_ASSERT_EQUAL_STRING("Project", tag);
	TEST_ASSERT_EQUAL_STRING("activity", attribute);
	TEST_ASSERT_EQUAL_STRING("Sleeping", value);
}

void callbackProjectClose(void* context, bool fTagClosed, char* tag, char* attribute, char* value)
{
	TEST_ASSERT(fTagClosed);
	TEST_ASSERT_EQUAL_STRING("Project", tag);
	TEST_ASSERT_EQUAL_STRING("", attribute);
	TEST_ASSERT_EQUAL_STRING("", value);
}

void callbackProjectsClose(void* context, bool fTagClosed, char* tag, char* attribute, char* value)
{
	TEST_ASSERT(fTagClosed);
	TEST_ASSERT_EQUAL_STRING("Projects", tag);
	TEST_ASSERT_EQUAL_STRING("", attribute);
	TEST_ASSERT_EQUAL_STRING("", value);
}

void testParse(void)
{
	char input[100] = "";

	XMLParser xmlParser = XMLParser();

	strcpy(input, "<Projects>\n");
	xmlParser.setCallback(callbackProjectsOpen, NULL);
	for (unsigned int i=0; i<strlen(input); i++)
	{
		xmlParser.parse(input[i]);
	}

	strcpy(input, "<Project ");
	xmlParser.setCallback(callbackProjectOpen, NULL);
	for (unsigned int i=0; i<strlen(input); i++)
	{
		xmlParser.parse(input[i]);
	}

	strcpy(input, " webUrl=\"http://ci.test.com/job/Job%20-%20One/\"");
	xmlParser.setCallback(callbackWebUrl, NULL);
	for (unsigned int i=0; i<strlen(input); i++)
	{
		xmlParser.parse(input[i]);
	}

	strcpy(input, " name=\"Job - One\"");
	xmlParser.setCallback(callbackName, NULL);
	for (unsigned int i=0; i<strlen(input); i++)
	{
		xmlParser.parse(input[i]);
	}

	strcpy(input, " lastBuildLabel=\"61\"");
	xmlParser.setCallback(callbacLastBuildLabel, NULL);
	for (unsigned int i=0; i<strlen(input); i++)
	{
		xmlParser.parse(input[i]);
	}

	strcpy(input, " lastBuildTime=\"2012-06-28T16:28:00Z\"");
	xmlParser.setCallback(callbackLastBuildTime, NULL);
	for (unsigned int i=0; i<strlen(input); i++)
	{
		xmlParser.parse(input[i]);
	}

	strcpy(input, " lastBuildStatus=\"Success\"");
	xmlParser.setCallback(callbackLastBuildStatus, NULL);
	for (unsigned int i=0; i<strlen(input); i++)
	{
		xmlParser.parse(input[i]);
	}

	strcpy(input, " activity=\"Sleeping\"");
	xmlParser.setCallback(callbackActivity, NULL);
	for (unsigned int i=0; i<strlen(input); i++)
	{
		xmlParser.parse(input[i]);
	}

	strcpy(input, "/>\n");
	xmlParser.setCallback(callbackProjectClose, NULL);
	for (unsigned int i=0; i<strlen(input); i++)
	{
		xmlParser.parse(input[i]);
	}

	strcpy(input, "</Projects>\n");
	xmlParser.setCallback(callbackProjectsClose, NULL);
	for (unsigned int i=0; i<strlen(input); i++)
	{
		xmlParser.parse(input[i]);
	}
}

void callbackBogusMarkup(void* context, bool fTagClosed, char* tag, char* attribute, char* value)
{
	TEST_ASSERT_FALSE(fTagClosed);
	TEST_ASSERT_EQUAL_STRING("", tag);
	TEST_ASSERT_EQUAL_STRING("", attribute);
	TEST_ASSERT_EQUAL_STRING("", value);
}

void testParseBogusMarkup(void)
{
	char input[100] = "<<<>\n";
	XMLParser xmlParser = XMLParser();
	xmlParser.setCallback(callbackBogusMarkup, NULL);
	for (unsigned int i=0; i<strlen(input); i++)
	{
		xmlParser.parse(input[i]);
	}
}

void callbackBufferOverflow(void* context, bool fTagClosed, char* tag, char* attribute, char* value)
{
	TEST_ASSERT(fTagClosed);
	TEST_ASSERT_EQUAL_STRING("012345678901234567890123456789012345678", tag);
	TEST_ASSERT_EQUAL_STRING("", attribute);
	TEST_ASSERT_EQUAL_STRING("", value);
}

void testParseBufferOverflow(void)
{
	char input[100] = "<01234567890123456789012345678901234567890123456789012345678901234567890123456789/>";

	XMLParser xmlParser = XMLParser();
	xmlParser.setCallback(callbackBufferOverflow, NULL);
	for (unsigned int i=0; i<strlen(input); i++)
	{
		xmlParser.parse(input[i]);
	}
}
