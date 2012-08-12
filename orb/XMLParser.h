#ifndef	XMLPARSER_H
#define XMLPARSER_H

#define MAX_STRING_LEN  40

class XMLParser
{

public:
    XMLParser();

    void parse(char in);
    char* getTag();
    char* getAttribute();
    char* getValue();
    void (*getCallback())(void*, bool, char*, char*, char*);
    void setCallback(void (*function)(void*, bool, char*, char*, char*), void* data);

private:
    void addChar(char ch, char* str);
    void reset();

    void (*callback)(void*, bool, char*, char*, char*);
    void *context;

    bool fTagClosed;
    bool fTagStarting;
    bool fInAttribute;
    bool fInValue;

    char tag[MAX_STRING_LEN];
    char attribute[MAX_STRING_LEN];
    char value[MAX_STRING_LEN];
    char tmp[MAX_STRING_LEN];
};

#endif
