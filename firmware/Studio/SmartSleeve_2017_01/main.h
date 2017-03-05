/** \file
 *
 *  Header file for adc.c.
 */

#ifndef _MAIN_H_
#define _MAIN_H_

#include <avr/io.h>
#include <math.h>
#include <stdio.h>
#include <util/delay.h>
#include "adc.h"
#include "adc.h"
#include "timer.h"
#include "u8glib/u8g.h"

	/* Macros to make bit manipulation easier */
	#define set_bit(address,bit) (address |= (1<<bit))
	#define clear_bit(address,bit) (address &= ~(1<<bit))
	#define toggle_bit(address,bit) (address ^= (1<<bit))
	#define check_bit(address,bit) ((address & (1<<bit)) == (1<<bit))

	/* Pressure Curve Fit Power 2 Fit 11-28-2016 */
	#define P1 458.6    
	#define P2 -0.7116  
	#define P3 4.604    

	/* Rounding Factor */
	#define offset 2.5
	
	/* ADC Reference Voltage */
	#define VREF 2.56 
	
	enum{BATTERY_NORMAL, BATTERY_LOW, BATTERY_CRITICAL};
	
	/* Global Variables */
	int          sensorADC         = 0x00;  // ADC0
	int          batteryADC        = 0x01;  // ADC1
	
	unsigned int sensorValue       = 0;     // variable to store the value coming from the sensor
	float        sensorVoltage     = 0;
	unsigned int sensorPressure    = 0;
	unsigned int batteryValue      = 0;
	float        resistance        = 0;
	float        batteryVoltage    = 0;
	short        batterylevel      = BATTERY_NORMAL;
	unsigned int initPressure      = 0;
    
	
	char   displayADC[10];
	char   displayPressure[10];
	char   displayResistance[10];
	char   displayVoltage[10];
	char   displayBattery[10];
	char   displayScalePressure[10];
	
	u8g_t u8g;

	/* Function Prototypes: */
	void initBoard(void);
	void acquireSensorVoltage(void);
	void acquireBatteryVoltage(void);
	void calcPressure(void);
	int  roundPressure(void);
	void readSensors(void);
	void draw(void); 
	void drawBatteryLow(void);
	void disableDisplay(void);
	void enableDisplay(void);
	
	#ifdef DEBUG
		short reset = 0;
	#else
		short reset = 1;
	#endif
	
	

#endif

