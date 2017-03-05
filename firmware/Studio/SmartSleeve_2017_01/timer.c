
/** \file
 *
 *  Source file for 16 bit Timer1.
 */

#include "timer.h"

void Timer_Init(void)
{

    /* Clear Timer on Compare Match Mode */
    TCCR1B |= 1 << WGM12;

    /* Set Timer Clock Prescaler to 256 */
    TCCR1B |= 1 << CS12;
	//TCCR1B |= (1 << CS12 | 1 << CS10 );

    /* Set Timer Interval */
    //OCR1A   = 6250;  // 10 Hz
    //OCR1A   = 31250; // 2 Hz
	OCR1A   = 62500; // 1 Hz
    
   
}

void Timer_Enable(void)
{
    /* Timer 1 Output Compare A Match Interrupt Enable */
    TIMSK1 |= 1 << OCIE1A;
	
	 timervalue    = 0;
	 timerinterupt = 0;
	 
	 sei(); // enable interrupts
}

void Timer_Disable(void)
{
    /* Disable Timer 1*/
    TIMSK1 = 0;   
}

ISR(TIMER1_COMPA_vect)
{
    /* Do Something */
	timervalue++;
	if(timervalue > TIMEOUT){
		timerinterupt = 1;
	}
}





