#ifndef LED_DISPLAY_H
#define LED_DISPLAY_H

#include "LEDPulser.h"

#define STATUS_OK 0
#define STATUS_CONNECTION_ERROR 1
#define STATUS_NO_NETWORK  2

class LEDDisplay
{
    public:
        LEDDisplay();
        void setRGBPins(int redPin, int greenPin, int bluePin);
        void buildInProgress(bool lastBuildSuccess);
        void buildDone(bool success);
        void someBuildFailed();
        void status(int code);
        void test();
        void sleep();
        void refresh();
	private:
        LEDPulser redLEDPulser;
        LEDPulser greenLEDPulser;
        LEDPulser blueLEDPulser;
};

extern LEDDisplay Display;

#endif