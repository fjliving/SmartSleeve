
/** \file
 *
 *  Source file for ADC.
 */

#include "adc.h"


void ADC_Init(void)
{
    /*
     ADEN: Set to turn on ADC , by default it is turned off
     ADPS2: ADPS2 and ADPS0 set to make division factor 128 (125 kHz) 
    */
    ADCSRA  = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADCSRA |= (1 << ADEN);
}

unsigned int ADC_Sample(char Channel)
{
    /* set the ADC input channel */
    ADMUX = Channel; 
	//ADMUX |= (1 << REFS0) 
    ADMUX |= (1 << REFS0) | (1 << REFS1); //Internal 2.56V Voltage Reference with external capacitor on AREF pin

    /* Start Conversion */
    ADCSRA |= (1<<ADSC);

    /* Wait for conversion to complete */
    while (ADCSRA & (1<<ADSC)); 

    /* Return ADC Value */
    return ADC;  
}



