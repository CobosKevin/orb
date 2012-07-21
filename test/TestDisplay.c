#include "unity.h"

#include "MockArduinoUno.h"
#include "LEDPulser.h"
#include "LEDDisplay.h"

void setUp(void)
{
	ArduinoUno_SetPinToOutputMode_Expect(1);
	ArduinoUno_SetPinToOutputMode_Expect(2);
	ArduinoUno_SetPinToOutputMode_Expect(3);

	Display.setRGBPins(1,2,3);
}

void tearDown(void)
{
}

void testSetRGBPins(void)
{
	// Done part of the setup
}

void testTest(void)
{
	ArduinoUno_AnalogWrite_Expect(1, 255);
	ArduinoUno_Delay_Expect(400);
	ArduinoUno_AnalogWrite_Expect(1, 0);
	ArduinoUno_AnalogWrite_Expect(2, 255);
	ArduinoUno_Delay_Expect(400);
	ArduinoUno_AnalogWrite_Expect(2, 0);
	ArduinoUno_AnalogWrite_Expect(3, 255);
	ArduinoUno_Delay_Expect(400);
	ArduinoUno_AnalogWrite_Expect(3, 0);

	Display.test();
}

void testBuildInProgressSuccess(void)
{
	ArduinoUno_AnalogWrite_Expect(1, 0);
	Display.buildInProgress(true);
	ArduinoUno_AnalogWrite_Expect(2, 1);
	Display.refresh();
}

void testBuildInProgressFailure(void)
{
	ArduinoUno_AnalogWrite_Expect(2, 0);
	Display.buildInProgress(false);
	ArduinoUno_AnalogWrite_Expect(1, 1);
	Display.refresh();
}

void testBuildDoneSuccess(void)
{
	ArduinoUno_AnalogWrite_Expect(1, 0);
	ArduinoUno_AnalogWrite_Expect(2, 255);
	Display.buildDone(true);
	Display.refresh();
}

void testBuildDoneFailure(void)
{
	ArduinoUno_AnalogWrite_Expect(2, 0);
	ArduinoUno_AnalogWrite_Expect(1, 255);
	Display.buildDone(false);
	Display.refresh();
}

void testSleep(void)
{
	ArduinoUno_AnalogWrite_Expect(1, 0);
	ArduinoUno_AnalogWrite_Expect(2, 0);
	Display.sleep();
	Display.refresh();
}

void testStatusOK(void)
{
	ArduinoUno_AnalogWrite_Expect(3, 0);
	Display.status(STATUS_OK);
	Display.refresh();
}

void testStatusConnectionError(void)
{
	ArduinoUno_AnalogWrite_Expect(3, 1);
	ArduinoUno_AnalogWrite_Expect(3, 2);
	Display.status(STATUS_CONNECTION_ERROR);
	Display.refresh();
	Display.refresh();
}

void testStatusNoNetwork(void)
{
	ArduinoUno_AnalogWrite_Expect(3, 15);
	ArduinoUno_AnalogWrite_Expect(3, 30);
	Display.status(STATUS_NO_NETWORK);
	Display.refresh();
	Display.refresh();
}

void testSomeBuildFailed(void)
{
	ArduinoUno_AnalogWrite_Expect(2, 0);
	ArduinoUno_AnalogWrite_Expect(1, 15);
	ArduinoUno_AnalogWrite_Expect(1, 30);
	Display.someBuildFailed();
	Display.refresh();
	Display.refresh();
}
