#include "XMLParser.h"

#include <string.h>

XMLParser::XMLParser()
{
    callback = NULL;
    reset();
}

void (*XMLParser::getCallback())(void*, bool, char*, char*, char*)
{
    return callback;
}
void XMLParser::setCallback(void (*function)(void*, bool, char*, char*, char*), void* data)
{
    context = data;
    callback = function;
}

void XMLParser::reset()
{
    fInAttribute = false;
    fInValue = false;
    fTagClosed = false;

    memset(tag, 0, MAX_STRING_LEN);
    memset(attribute, 0, MAX_STRING_LEN);
    memset(value, 0, MAX_STRING_LEN);
    memset(tmp, 0, MAX_STRING_LEN);
}

void XMLParser::addChar(char ch, char* str)
{
    if (strlen(str) < MAX_STRING_LEN-1)
    {
        str[strlen(str)] = ch;
    }
}

void XMLParser::parse(char in)
{
    if (callback == NULL)
    {
        return;
    }

    switch (in)
    {
    case '<': // Tag starting
        memset(tmp, 0, strlen(tmp));
        break;

    case ' ':
        if (strlen(tag) == 0)
        {
            // Save tag name
            strcpy(tag, tmp);
            callback(context, false, tag, (char *)"", (char *)"");
        }

        if (fInValue)
        {
            addChar(in, tmp);
        }
        else
        {
            fInAttribute = true;
            memset(tmp, 0, strlen(tmp));
        }
        break;

    case '/':
        if (fInValue)
        {
            addChar(in, tmp);
        }
        else
        {
            fTagClosed = true;
        }
        break;

    case '>': // Tag ending
        if (strlen(tag) == 0)
        {
            strcpy(tag, tmp);
        }
        callback(context, fTagClosed, tag, (char *)"", (char *)"");
        reset();
        break;

    case '=':
        if (fInValue)
        {
            addChar(in, tmp);
        }
        else if (fInAttribute)
        {
            strcpy(attribute, tmp);
            memset(tmp, 0, strlen(tmp));
            fInAttribute = false;
        }
        break;

    case '"':
        if (fInValue)
        {
            strcpy(value, tmp);
            memset(tmp, 0, strlen(tmp));
            fInValue = false;

            // Process tag, attribute, value
            callback(context, false, tag, attribute, value);
        }
        else
        {
            fInValue = true;
        }
        break;

    case 10:
        // New line, reset
        reset();
        break;

    default:
        addChar(in, tmp);
    }

}


