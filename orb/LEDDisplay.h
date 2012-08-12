#ifndef LED_DISPLAY_H
#define LED_DISPLAY_H

#include "MegaBrite.h"

#define STATUS_OK                   0
#define STATUS_CONNECTION_ERROR     1
#define STATUS_NO_NETWORK           2
#define STATUS_SETUP_MODE           3
#define STATUS_NO_BUILDS            4
#define STATUS_ERROR_PARSING_BUILDS 5

class LEDDisplay
{
public:
    LEDDisplay();
    void buildInProgress(bool lastBuildSuccess);
    void buildDone(bool success);
    void someBuildFailed();
    void status(int code);
    void test();
    void sleep();
    void refresh();

private:
    MegaBrite megaBrite;
};

extern LEDDisplay Display;

#endif
