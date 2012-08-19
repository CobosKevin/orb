#include "MegaBrite.h"

#include "ArduinoUno.h"
#include <binary.h>

#define LED_FADE_STEP_SLOW  2
#define LED_FADE_STEP_FAST  32

#define FADE_UP         1
#define FADE_DOWN       -1

#define CLOCK_PIN  13U
#define ENABLE_PIN 10U
#define LATCH_PIN   9U
#define DATA_PIN   11U

MegaBrite::MegaBrite()
{
	for (int i =0; i < 3; i++)
	{
		pulse[i] = false;
		toggle[i] = false;
		step[i] = LED_FADE_STEP_SLOW;
		value[i] = 0;
		direction[i] = FADE_UP;
	}	

    previousRed = -1;
    previousGreen = -1;
    previousBlue = -1;
}

/*
	Does all the hardware related outside of the constructor
	otherwise this generates an exception during compile time because of the mocks
*/
void MegaBrite::setup()
{
	ArduinoUno_SetPinToOutputMode(CLOCK_PIN);
	ArduinoUno_SetPinToOutputMode(ENABLE_PIN);
	ArduinoUno_SetPinToOutputMode(LATCH_PIN);
	ArduinoUno_SetPinToOutputMode(DATA_PIN);

	ArduinoUno_SetSPIControlRegister((1<<SPE)|(1<<MSTR)|(0<<SPR1)|(0<<SPR0));
	ArduinoUno_DigitalWrite(LATCH_PIN, LOW);
	ArduinoUno_DigitalWrite(ENABLE_PIN, LOW);

	setControlRegister();
	setPWMRegister();
}

void MegaBrite::pulseOn(unsigned short led, unsigned short speed)
{
	if (speed == SLOW)
	{
		step[led] = LED_FADE_STEP_SLOW;
	}
	else
	{
	    step[led] = LED_FADE_STEP_FAST;		
	}
	toggleOff(led);
	pulse[led] = true;
}

void MegaBrite::pulseOff(unsigned short led)
{
	pulse[led] = false;
}

void MegaBrite::toggleOn(unsigned short led)
{
	pulseOff(led);
	toggle[led] = true;
}

void MegaBrite::toggleOff(unsigned short led)
{
	toggle[led] = false;
}

void MegaBrite::on(unsigned short led)
{
	pulseOff(led);
	toggleOff(led);
	value[led] = 1023;
}

void MegaBrite::off(unsigned short led)
{
	pulseOff(led);
	toggleOff(led);
	value[led] = 0;
}

void MegaBrite::animate()
{
	bool toggled = false;
    static long lastToggled = 0;

	for (int i =0; i < 3; i++)
	{
	    if (pulse[i])
	    {
	        value[i] += (direction[i] * step[i]);	        

	        if (value[i] >= 1024)
	        {
	        	value[i] = 1023;
	            direction[i] = FADE_DOWN;
	        }
	        if (value[i] <= 0)
	        {
	        	value[i] = 0;
	            direction[i] = FADE_UP;
	        }
	    }		
	}

	if (ArduinoUno_Millis() - lastToggled > 1000)
	{
		for (int i =0; i < 3; i++)
		{
		    if (toggle[i])
		    {
		        if (value[i] == 0 && !toggled)
		        {
		        	value[i] = 1023;
		        	toggled = true;
	                lastToggled = ArduinoUno_Millis();
		        }
		        else
		        {
		        	value[i] = 0;
		        }
		    }		
		}		
	}

	setPWMRegister();
}

void MegaBrite::setPWMRegister()
{
	if (previousRed != value[RED] || previousGreen != value[GREEN] || previousBlue != value[BLUE])
	{
		// Write to the PWM registers
		sendPacket(0, value[RED], value[GREEN], value[BLUE]);
		ArduinoUno_DelayMicroseconds(15);
	    ArduinoUno_DigitalWrite(LATCH_PIN, HIGH); // LATCH_PIN data into registers
	    ArduinoUno_DelayMicroseconds(15);
	    ArduinoUno_DigitalWrite(LATCH_PIN, LOW);

	    previousRed = value[RED];
	    previousGreen = value[GREEN];
	    previousBlue = value[BLUE];
	}
}

/*
	Send command to set control register.
	http://docs.macetech.com/doku.php/megabrite
*/
void MegaBrite::setControlRegister()
{
	sendPacket(1, B1111111, B1100100, B1100100);
	ArduinoUno_DelayMicroseconds(15);
    ArduinoUno_DigitalWrite(LATCH_PIN, HIGH); // LATCH_PIN data into registers
    ArduinoUno_DelayMicroseconds(15);
    ArduinoUno_DigitalWrite(LATCH_PIN, LOW);    
}

/*
    |---------------------------\   |----------------------------|   |---------------------------|  |----------------------------|
	0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16	17	18	19	20	21	22	23	24	25	26	27	28	29	30	31
	P00 P01 P02 P03 P04 P05 P06 P07 P08 P09 P10 P11 P12 P13 P14 P15 P16 P17 P18 P19 P20 P21 P22 P23 P24 P25 P26 P27 P28 P29 0	X
                GREEN                           RED                          BLUE
	D00 D01 D02 D03 D04 D05 D06 Clk	Clk X	D10 D11 D12 D13 D14 D15 D16  X	X	X	D20 D21 D22 D23 D24 D25 D26 X	ATB	ATB	1	X
*/
void MegaBrite::sendPacket(int mode, int red, int green, int blue) 
{ 
    // 0100 0110
    ArduinoUno_SetSPIDataRegister(mode << 6 | blue >> 4);
    while(!(ArduinoUno_GetSPIStatusRegister() & (1<<SPIF)));
    // 0100 0001
    ArduinoUno_SetSPIDataRegister(blue << 4 | red >> 6);
    while(!(ArduinoUno_GetSPIStatusRegister() & (1<<SPIF)));
    // 1111 1100
    ArduinoUno_SetSPIDataRegister(red << 2 | green >> 8);
    while(!(ArduinoUno_GetSPIStatusRegister() & (1<<SPIF)));
    // 0110 0100
    ArduinoUno_SetSPIDataRegister(green);
    while(!(ArduinoUno_GetSPIStatusRegister() & (1<<SPIF)));
}
