/*
 * ad9833.c
 *
 * Created: 2/23/2021 3:16:01 PM
 * make LFO waveforms on AD9833 SPI FG using incoming CV to C pin 01
 */ 

 /*writing frequency on AD9833 NOTES. 
 AD9833 muxes control and freq settings in 4 bytes
logic in the same word.
So for LSB,L the right nibble must be x5
for MSB,L, the right nibble must always be x4
otherwise FG chip will crash.
*/

/*before compiling: check SPI3.h to 
make sure SPI pins are correctly identified.
*/
#define F_CPU 16000000UL
#define BAUD 9600
#define BAUD_TOL 2
#include <avr/io.h>
#include "spi3.h"
#include <stdint.h>
#include <stddef.h>
#include <util/delay.h>
#include "ADClib.h"
#include "stdio_setup.h"
#include <stdio.h>
#include <math.h>

void SPI_TransferTx16(uint8_t a, uint8_t b);
 
 
//static unsigned char MSB_phase = 0;
//static unsigned char LSB_phase = 0;

uint32_t desired_frequency = 0;
uint8_t z = 0;

void set_phase(uint32_t desired_phase);
uint8_t gethexcount(uint32_t n);
uint16_t get_ad_freq(uint16_t freq);


int main(void)
{
//set up stdio_setup
UartInit();
	
//initialize SPI
init_spi_master();
_delay_ms(100);
spi_mode(2);
 _delay_ms(15);
DESELECT();
SELECT();

SPI_TransferTx16(33,0); //control 16 bits w reset 2 byte freq write
	
		uint16_t CV = 0;
		uint16_t adfreq;
		
		ADC_init();
		
		uint8_t LSB_L = 0;
		uint8_t LSB_R = 0;
   
//write to chip initial freq of 0 and release reset
SPI_TransferTx16(0x40,0x00); //freq LSB (L,R) // adjust freq slightly includes control in bits 29 to 32
SPI_TransferTx16(0x40,0x00); // freq MSB (L,R) // adjust freq greatly includes control in bits 29 to 32    
SPI_TransferTx16(192,0);  //phase val 0
SPI_TransferTx16(32,0); // exit reset mode by issuing conseq MSB LSB freq write. sin wave
//SPI_TransferTx16(32,0); // exit reset mode by issuing conseq MSB LSB freq write. Tri wave

    
    while (1) 
    {   

	 
		CV = analogRead10bit();
	
      //   printf("    in main: CV is: %d\n\r",CV);
		
        
		
	     adfreq = get_ad_freq(CV);
		 //_delay_ms(20);
		 //printf("adfreq is: %x\n\r",adfreq);
		//_delay_ms(200);
		//SPI_TransferTx16(33,0); //control 16 bits w reset 2 byte freq write  LEAVE OUT OF WHILE1 LOOP
	
         adfreq = adfreq + 0x4000;	
		 LSB_L = (adfreq & 0xFF00) >> 8;
		 LSB_R = adfreq & 0x00FF;

		 	
          //printf("LSB_L is: 0x%x%x LSB_R \n\r",LSB_L,LSB_R);         
         
		 // _delay_ms(500);
		    //_delay_ms(1000);
		    //next line produces 400hz use to test.	
			//SPI_TransferTx16(0x50,0xC7); //freq LSB (L,R) // adjust freq slightly includes control in bits 29 to 32 
			SPI_TransferTx16(LSB_L,LSB_R); //freq LSB (L,R) // adjust freq slightly includes control in bits 29 to 32 
			SPI_TransferTx16(0x40,0x00); // freq MSB (L,R) // adjust freq greatly includes control in bits 29 to 32
			SPI_TransferTx16(192,0);  //phase val 0
	       SPI_TransferTx16(32,0); // exit reset mode by issuing conseq MSB LSB freq write. sin wave

	}
}



/*******************************************
input desired frequency as unsigned 32 int
function creates the 4 static bytes needed to set AD9833 freq.
you'll need to put that into 4 SPI write()s
which is not provided by this function.
*******************************************/

uint16_t get_ad_freq(uint16_t freq)
{
/*freq is 16 bit value of frequency you want to have 9833 create
return value is 32 bit value of "ad9833" style frequency, LSB followed by MSB
IMPORTANT!!!



return value does not have control bits in pos 15,14, 
required by AD9833 freq reg,
so you have to add those s
somewhere else */

//printf("In getadfreq function, freq is: %d\n\r",freq);
//uint16_t adfreqround = 0;
float bigfreq = freq;

float bits =  268435456; // 2^28
float clock = 25000000; // clock
//uint32_t result = 0;

float ad9833freq = bigfreq * bits;
float xxx = (ad9833freq / clock);

//adfreqround = round(xxx);
//printf("In getadfreq function, adreqround is: %x\n\r",adfreqround);
//printf("In getadfreq function, xxx is: %f\n\r",xxx);
 
//return adfreqround;
return xxx;
}

uint8_t gethexcount(uint32_t n)
{
	//counts # of hex digits for incoming # n
	uint8_t count;

	count = 0;

	
	while (n / 16 > 0)
	{
		count++;
		n = n / 16;
	}
	
	if (n % 16 != 0)
	{
		return count + 1;
	}
	else
	{
		return count;
	}
}

void set_phase(uint32_t desired_phase)
{
//desired_phase = desired_phase & 0b0000111111111111;
//MSB_phase = desired_phase >> 8;
//LSB_phase = MSB_phase & 0b00001111;	
}

void SPI_TransferTx16(uint8_t a, uint8_t b)
{	
	SELECT();
	SPI_SendByte(a);

	SPI_SendByte(b);
	DESELECT();	
}

 