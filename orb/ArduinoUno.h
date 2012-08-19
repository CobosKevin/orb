#ifndef ARDUINOUNO_H
#define ARDUINOUNO_H

/*
	Thin wrapper to native Arduino functions so that they can be easily mocked
*/

#ifndef Arduino_h
typedef unsigned char uint8_t;	
typedef uint8_t byte;
#define HIGH 0x1
#define LOW  0x0
#endif

void ArduinoUno_SetPinToOutputMode(unsigned short pin);
void ArduinoUno_AnalogWrite(unsigned short pin, unsigned short level);
void ArduinoUno_DigitalWrite(unsigned short pin, unsigned short level);
void ArduinoUno_Delay(long ms);
void ArduinoUno_DelayMicroseconds(long us);
long ArduinoUno_Millis();

/* 
	Write to the SPI Control Register
	Bit 6 – SPE: SPI Enable
		When the SPE bit is written to one, the SPI is enabled. This bit must be set to enable any SPI operations.
	Bit 4 – MSTR: Master/Slave Select
		This bit selects Master SPI mode when written to one, and Slave SPI mode when written logic zero. 
		If SS is configured as an input and is driven low while MSTR is set, MSTR will be cleared, and SPIF in SPSR will become set. 
		The user will then have to set MSTR to re-enable SPI Master mode.
	Bits 1, 0 – SPR1, SPR0: SPI Clock Rate Select 1 and 0
		These two bits control the SCK rate of the device configured as a Master. SPR1 and SPR0 have no effect on the Slave. 
		The relationship between SCK and the Oscillator Clock frequency fosc is shown in the following table:
		http://www.atmel.com/Images/doc8161.pdf
*/
void ArduinoUno_SetSPIControlRegister(byte value);

#ifndef SPR0
#define SPR0 0
#define SPR1 1
#define CPHA 2
#define CPOL 3
#define MSTR 4
#define DORD 5
#define SPE  6
#define SPIE 7
#endif


/*
	Write to the SPI Data Register
	The SPI Data Register is a read/write register used for data transfer between the Register File and the SPI Shift Register. 
	Writing to the register initiates data transmission. Reading the register causes the Shift Register Receive buffer to be read.
*/
void ArduinoUno_SetSPIDataRegister(byte value);

/*
	Returns the SPI Status Register
	Bit 7 – SPIF: SPI Interrupt Flag
		When a serial transfer is complete, the SPIF Flag is set. An interrupt is generated if SPIE in SPCR is set and global 
		interrupts are enabled. If SS is an input and is driven low when the SPI is in Master mode, this will also set the SPIF Flag. 
		SPIF is cleared by hardware when executing the corresponding interrupt handling vector. Alternatively, the SPIF bit is 
		cleared by first reading the SPI Status Register with SPIF set, then accessing the SPI Data Register (SPDR).
*/
byte ArduinoUno_GetSPIStatusRegister();

#ifndef SPIF
#define SPIF 7
#endif

#endif