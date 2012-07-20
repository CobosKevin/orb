#include "unity.h"

#include "LEDPulser.h"

#include "MockArduinoUno.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void testConstructor(void)
{
	ArduinoUno_SetPinToOutputMode_Expect(1);
	LEDPulser pulser = LEDPulser(1);
}

void testDisable(void)
{
	ArduinoUno_SetPinToOutputMode_Expect(1);
	LEDPulser pulser = LEDPulser(1);
	pulser.disable();
}

void testEnable(void)
{
	ArduinoUno_SetPinToOutputMode_Expect(1);
	LEDPulser pulser = LEDPulser(1);
	pulser.enable();
}

void testPulseWhenNeverTurnedOn(void)
{
	ArduinoUno_SetPinToOutputMode_Expect(1);
	LEDPulser pulser = LEDPulser(1);
	pulser.pulse();
}

void testPulseWhenEnable(void)
{
	ArduinoUno_SetPinToOutputMode_Expect(1);
	LEDPulser pulser = LEDPulser(1);
	pulser.enable();
	ArduinoUno_AnalogWrite_Expect(1, 1);
	pulser.pulse();
	ArduinoUno_AnalogWrite_Expect(1, 2);
	pulser.pulse();
	ArduinoUno_AnalogWrite_Expect(1, 3);
	pulser.pulse();
}

void testPulseReverseWhenReachMaxOrMin(void)
{
	ArduinoUno_SetPinToOutputMode_Expect(1);
	LEDPulser pulser = LEDPulser(1);
	pulser.enable();

	for (int i=1; i<=255; i++)
	{
		ArduinoUno_AnalogWrite_Expect(1, i);
		pulser.pulse();		
	}

	for (int i=254; i>=0; i--)
	{
		ArduinoUno_AnalogWrite_Expect(1, i);
		pulser.pulse();		
	}

	for (int i=1; i<=255; i++)
	{
		ArduinoUno_AnalogWrite_Expect(1, i);
		pulser.pulse();		
	}
}
