#include "LEDDisplay.h"

#include "ArduinoUno.h"

LEDDisplay::LEDDisplay()
{
    megaBrite = MegaBrite();
}

void LEDDisplay::refresh()
{
    megaBrite.animate();
}

void LEDDisplay::test()
{
    megaBrite.on(RED);
    ArduinoUno_Delay(400);
    megaBrite.off(RED);
    megaBrite.on(GREEN);
    ArduinoUno_Delay(400);
    megaBrite.off(GREEN);
    megaBrite.on(BLUE);
    ArduinoUno_Delay(400);
    megaBrite.off(BLUE);
}

void LEDDisplay::buildInProgress(bool lastBuildSuccess = true)
{
    if (lastBuildSuccess)
    {
        megaBrite.off(RED);
        megaBrite.pulseOn(GREEN, SLOW);
    }
    else
    {
        megaBrite.off(GREEN);
        megaBrite.pulseOn(RED, SLOW);
    }
}

void LEDDisplay::buildDone(bool success = true)
{
    if (success)
    {
        megaBrite.off(RED);
        megaBrite.on(GREEN);
    }
    else
    {
        megaBrite.off(GREEN);
        megaBrite.on(RED);
    }

}

void LEDDisplay::sleep()
{
    megaBrite.off(GREEN);
    megaBrite.off(RED);
}

void LEDDisplay::status(int code)
{
    switch(code)
    {
    case STATUS_OK:
        megaBrite.off(BLUE);
        break;
    case STATUS_CONNECTION_ERROR:
        megaBrite.pulseOn(BLUE, SLOW);
        break;
    case STATUS_NO_NETWORK:
        megaBrite.pulseOn(BLUE, FAST);
        break;
    case STATUS_SETUP_MODE:
        megaBrite.on(BLUE);
        break;
    case STATUS_NO_BUILDS:
    case STATUS_ERROR_PARSING_BUILDS:
        megaBrite.toggleOn(RED);
        megaBrite.toggleOn(GREEN);
        break;
    default:
        megaBrite.off(BLUE);
    }
}

void LEDDisplay::someBuildFailed()
{
    megaBrite.off(GREEN);
    megaBrite.pulseOn(RED, FAST);
}

LEDDisplay Display;
