#include <avr/io.h>
#include "ADClib.h"

void ADC_init()
{
	
	//USING THIS!
	//next line sets which C port is ADC in.
	//may not make sense to use all 8 pins as inputs...
	//but that's what I have here
	// NExt line configures all PORTC bits to an input
	DDRC = 0b00000000;


	//UNOS have AVCC pulled high (see schematic) but you may need to change that depending on app.
	//MSB 2 bits set ADC reference voltage.  00 external  01 AVCC   10 reserved  11 internal 2.56V
	//LSB 5 bits set which pin to use for ADC. First 3 LSB just choose pin, but next 2 LSB chooses variations,
	//including gain shifts.
	// other bit justifies the ADC output in registers
	// Leave it at zero or code won't work
	// next line:
	// Configure ADC to be right justified, use AVCC as reference, and select ADC1 as ADC input
	ADMUX = 0b01000001;

	// Enable the ADC and set the prescaler to max value (128)
	ADCSRA = 0b11000111;
}

unsigned char analogRead8bit()
{
	unsigned char x;
	
	// Start an ADC conversion by setting ADSC bit (bit 6)
	ADCSRA = ADCSRA | (1 << ADSC);
	
	// Wait until the ADSC bit has been cleared
	while(ADCSRA & (1 << ADSC));
	x = ADCH;
	
	
	return x;
}

unsigned int analogRead10bit()
{
	unsigned int y;
	unsigned int a;
	
	//can;t get this to work unless I call the settings below twice.
	//no idea why!
	
	unsigned char fake;
	
	// Start an ADC conversion by setting ADSC bit (bit 6)
	ADCSRA = ADCSRA | (1 << ADSC);
	
	// Wait until the ADSC bit has been cleared
	while(ADCSRA & (1 << ADSC));
	fake = ADCH;

	//OK with that called we can now call it again and it will work.
	
	// Start an ADC conversion by setting ADSC bit (bit 6)
	ADCSRA = ADCSRA | (1 << ADSC);
	
	// Wait until the ADSC bit has been cleared
	while(ADCSRA & (1 << ADSC));
	
	// make the output a 10 bit int.
	y = ADCH << 8;
	//y = ADCL;
	a = y + ADCL;

	return a;
}
