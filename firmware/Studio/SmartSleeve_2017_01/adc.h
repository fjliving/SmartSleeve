/** \file
 *
 *  Header file for adc.c.
 */

#ifndef _ADC_H_
#define _ADC_H_

	/* Includes: */
		#include <avr/io.h>

	/* Function Prototypes: */
		void ADC_Init(void);
		unsigned int ADC_Sample(char);

#endif

