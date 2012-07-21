#include "LEDDisplay.h"

#include "ArduinoUno.h"

LEDDisplay::LEDDisplay()
{
}

void LEDDisplay::setRGBPins(int redPin, int greenPin, int bluePin)
{
	redLEDPulser = LEDPulser(redPin);
	greenLEDPulser = LEDPulser(greenPin);
	blueLEDPulser = LEDPulser(bluePin);	
}

void LEDDisplay::refresh()
{
    redLEDPulser.pulse();
    greenLEDPulser.pulse();
    blueLEDPulser.pulse();
}

void LEDDisplay::test() {
    redLEDPulser.on();
    ArduinoUno_Delay(400);
    redLEDPulser.off();
    greenLEDPulser.on();
    ArduinoUno_Delay(400);
    greenLEDPulser.off();
	blueLEDPulser.on();
	ArduinoUno_Delay(400);
    blueLEDPulser.off();
}

void LEDDisplay::buildInProgress(bool lastBuildSuccess = true)
{
	if (lastBuildSuccess) 
    {
		redLEDPulser.off();
		greenLEDPulser.enable();
	} 
    else 
    {
		greenLEDPulser.off();
        redLEDPulser.slow();
		redLEDPulser.enable();
	}
}

void LEDDisplay::buildDone(bool success = true)
{
    if (success) 
    {
        redLEDPulser.off();
        greenLEDPulser.on();
    } 
    else 
    {
        greenLEDPulser.off();
        redLEDPulser.on();
    }            

}

void LEDDisplay::sleep()
{
    redLEDPulser.off();
	greenLEDPulser.off();
}

void LEDDisplay::status(int code)
{
	switch(code)
	{
		case STATUS_OK:
			blueLEDPulser.off();
			break;
		case STATUS_CONNECTION_ERROR:
	        blueLEDPulser.slow();
	        blueLEDPulser.enable();
			break;
		case STATUS_NO_NETWORK:
	        blueLEDPulser.fast();
	        blueLEDPulser.enable();
			break;
		default:
			blueLEDPulser.off();
	}
}

void LEDDisplay::someBuildFailed()
{
	greenLEDPulser.off();
	redLEDPulser.fast();
	redLEDPulser.enable();
}

LEDDisplay Display;
