/** \file
 *
 *  Header file for timer.c.
 */

#ifndef _TIMER_H_
#define _TIMER_H_

	/* Includes: */
		#include <avr/io.h>
        #include <avr/interrupt.h>
		
		/* Timeout [Secs] */
		#define TIMEOUT 900  // 15 mins

	/* Function Prototypes: */
		void Timer_Init(void);
        void Timer_Enable(void);
        void Timer_Disable(void);
		
		unsigned int timervalue;
		unsigned int timerinterupt;

#endif

