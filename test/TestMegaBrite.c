#include "unity.h"
#include "MockArduinoUno.h"
#include "MegaBrite.h"
#include <binary.h>

void setUp(void)
{

}

void tearDown(void)
{
}

void expectGreenWrite(int green)
{
	ArduinoUno_Millis_ExpectAndReturn(0);
	ArduinoUno_SetSPIDataRegister_Expect(0);
	ArduinoUno_GetSPIStatusRegister_ExpectAndReturn(B10000000);
	ArduinoUno_SetSPIDataRegister_Expect(0);
	ArduinoUno_GetSPIStatusRegister_ExpectAndReturn(B10000000);
	ArduinoUno_SetSPIDataRegister_Expect(green >> 8);
	ArduinoUno_GetSPIStatusRegister_ExpectAndReturn(B10000000);
	ArduinoUno_SetSPIDataRegister_Expect(green);
	ArduinoUno_GetSPIStatusRegister_ExpectAndReturn(B10000000);

	ArduinoUno_DelayMicroseconds_Expect(15);
	ArduinoUno_DigitalWrite_Expect(9U,1);
	ArduinoUno_DelayMicroseconds_Expect(15);
	ArduinoUno_DigitalWrite_Expect(9U,0);	
}

void testSetup(void)
{
	MegaBrite megaBrite = MegaBrite();

	/*
		Setup sequence
	*/
	ArduinoUno_SetPinToOutputMode_Expect(13U);
	ArduinoUno_SetPinToOutputMode_Expect(10U);
	ArduinoUno_SetPinToOutputMode_Expect(9U);
	ArduinoUno_SetPinToOutputMode_Expect(11U);

	ArduinoUno_SetSPIControlRegister_Expect(80);

	ArduinoUno_DigitalWrite_Expect(9U, 0);
	ArduinoUno_DigitalWrite_Expect(10U, 0);

	ArduinoUno_SetSPIDataRegister_Expect(B01000110);
	ArduinoUno_GetSPIStatusRegister_ExpectAndReturn(B10000000);
	ArduinoUno_SetSPIDataRegister_Expect(B01000001);
	ArduinoUno_GetSPIStatusRegister_ExpectAndReturn(B10000000);
	ArduinoUno_SetSPIDataRegister_Expect(B11111100);
	ArduinoUno_GetSPIStatusRegister_ExpectAndReturn(B10000000);
	ArduinoUno_SetSPIDataRegister_Expect(B01100100);
	ArduinoUno_GetSPIStatusRegister_ExpectAndReturn(B10000000);

	ArduinoUno_DelayMicroseconds_Expect(15);
	ArduinoUno_DigitalWrite_Expect(9U,1);
	ArduinoUno_DelayMicroseconds_Expect(15);
	ArduinoUno_DigitalWrite_Expect(9U,0);

	ArduinoUno_SetSPIDataRegister_Expect(0);
	ArduinoUno_GetSPIStatusRegister_ExpectAndReturn(B10000000);
	ArduinoUno_SetSPIDataRegister_Expect(0);
	ArduinoUno_GetSPIStatusRegister_ExpectAndReturn(B10000000);
	ArduinoUno_SetSPIDataRegister_Expect(0);
	ArduinoUno_GetSPIStatusRegister_ExpectAndReturn(B10000000);
	ArduinoUno_SetSPIDataRegister_Expect(0);
	ArduinoUno_GetSPIStatusRegister_ExpectAndReturn(B10000000);

	ArduinoUno_DelayMicroseconds_Expect(15);
	ArduinoUno_DigitalWrite_Expect(9U,1);
	ArduinoUno_DelayMicroseconds_Expect(15);
	ArduinoUno_DigitalWrite_Expect(9U,0);

	megaBrite.setup();
}

void testOn(void)
{
	MegaBrite megaBrite = MegaBrite();

	expectGreenWrite(1023);

	megaBrite.on(1);	
	megaBrite.animate();
}

void testOff(void)
{
	MegaBrite megaBrite = MegaBrite();

	// Turn on first because of optimziation to not set device
	// to previous state

	expectGreenWrite(1023);

	megaBrite.on(1);	
	megaBrite.animate();

	expectGreenWrite(0);

	megaBrite.off(1);	
	megaBrite.animate();
}

void testPulseOn(void)
{
	MegaBrite megaBrite = MegaBrite();

	megaBrite.pulseOn(1, 0);	

	expectGreenWrite(B00000010);

	megaBrite.animate(); 

	expectGreenWrite(B00000100);

	megaBrite.animate();
}

void testPulseReverseWhenReachMaxOrMin(void)
{
	MegaBrite megaBrite = MegaBrite();

	megaBrite.pulseOn(1, 0);	

	for (int i=2; i<1024; i+=2)
	{
		expectGreenWrite(i);
		megaBrite.animate();
	}


	for (int i=1023; i==0; i-=2)
	{
		expectGreenWrite(i);
		megaBrite.animate();
	}
}
