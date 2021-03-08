#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
 
//multiplying dividing and printing long longs in C

uint8_t gethexcount(uint32_t n);

int main(void)
{

float freq = 0;
freq = 400;
uint32_t bits =  268435456; // 2^28
uint32_t clock = 25000000; // clock
 

double bignum = freq * bits;
//bignum5 = 400 * bignum; won't work. 
//have to declare everything as a typecast variable.
float ad9833freq = 0;
ad9833freq = bignum / clock;
uint32_t ad9833freq_rounded = 0;
ad9833freq_rounded = round(ad9833freq);
//printf("result in dec is: %f \n\r",ad9833freq);
//printf("result in dec rounded is: %d \n\r",ad9833freq_rounded);
uint8_t getcount = 0;
getcount = gethexcount(ad9833freq_rounded);
//printf("result freq in hex rounded is: %x \n\r",ad9833freq_rounded);
//printf("Getcount is: %d \n",getcount);

//printf("result in hex ready to go is: %8x \n\r",ad9833freq_rounded << 16 - getcount);

}

uint8_t gethexcount(uint32_t n)
{
//counts # of hex digits for incoming # n
uint8_t count;
uint8_t div;
        count = 0;
		div = 0;
        
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
				 
  
 