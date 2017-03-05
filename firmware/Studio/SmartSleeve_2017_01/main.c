/*
 * SmartSleeve_2017_01.c
 *
 * Created: 2/13/2017 1:10:55 PM
 * Author : Fred
 */ 

#include "main.h"

//#define DEBUG

int main(void)
{
	/* select minimal prescaler (max system speed) */
	CLKPR = 0x80;
	CLKPR = 0x00;
	 
	/* Initialize Peripherals */
	ADC_Init();
	initBoard();
	
	/* get initial voltage */
	calcPressure();
	initPressure = sensorPressure;

	while (1)
	{
		readSensors(); 
		_delay_ms(200);
	}
}

void acquireSensorVoltage(void) {
	
	// read the value from the sensor:
	float value = 0;
	for (int i=0; i < 10; i++)
		value += ADC_Sample(sensorADC);  
	
	sensorValue = value/10.0;
	sensorVoltage = sensorValue * VREF / 1023.0;
}


void acquireBatteryVoltage(void) {
    // read the value from the sensor:
    batteryValue    = ADC_Sample(batteryADC);
    batteryVoltage  = batteryValue * VREF / 1023.0;
	batteryVoltage *= 2.3986;
}

void calcPressure(void) {
	/* read the voltage for ADC */
	acquireSensorVoltage();

	/* compute the resistance from voltage */
	/* Board 1 */
	//resistance  = (2.4375*sensorVoltage + 4.201796)*-4020.0;
	//resistance /= (1.4825 * sensorVoltage - 4.20179623);
	
	/* Board 2*/
	//resistance  = (4.875*sensorVoltage + 8.4709784)*-4020.0;
	//resistance /= (2.965 * sensorVoltage - 8.47097843);
	
	/* Rev 1*/
	resistance  = (1.391*sensorVoltage + 3.479682641)*-8200.0;
	resistance /= (1.147 * sensorVoltage - 3.479682641);

	/* compute pressure from resistance (KOhms) */
	sensorPressure =  P1 * pow(resistance / 1000.0, P2) + P3;

	/* max of pressure */
	#ifndef DEBUG
		if (sensorPressure >= 99) sensorPressure = 99;
		if (sensorPressure <= 20) reset = 1;
	#endif
}


int roundPressure(void) {
	int retval = 0;
	
	if (sensorPressure >= (10 - offset) && sensorPressure < (10 + offset)) retval = 10;
	else if (sensorPressure >= (15 - offset) && sensorPressure < (15 + offset)) retval = 15;
	else if (sensorPressure >= (20 - offset) && sensorPressure < (20 + offset)) retval = 20;
	else if (sensorPressure >= (25 - offset) && sensorPressure < (25 + offset)) retval = 25;
	else if (sensorPressure >= (30 - offset) && sensorPressure < (30 + offset)) retval = 30;
	else if (sensorPressure >= (35 - offset) && sensorPressure < (35 + offset)) retval = 35;
	else if (sensorPressure >= (40 - offset) && sensorPressure < (40 + offset)) retval = 40;
	else if (sensorPressure >= (45 - offset) && sensorPressure < (45 + offset)) retval = 45;
	else if (sensorPressure >= (50 - offset) && sensorPressure < (50 + offset)) retval = 50;
	else if (sensorPressure >= (55 - offset) && sensorPressure < (55 + offset)) retval = 55;
	else if (sensorPressure >= (60 - offset) && sensorPressure < (60 + offset)) retval = 60;
	else if (sensorPressure >= (65 - offset) && sensorPressure < (65 + offset)) retval = 65;
	else if (sensorPressure >= (65 - offset)) retval = 99;
	else retval = 0;

	return retval;
}

void readSensors(void) {
	// -------------- read the sensors voltage ------------------- //
	calcPressure();
	acquireBatteryVoltage();
	
	// ------------------ check battery levels ------------------- //
	if(timerinterupt == 1 || batteryVoltage < 4.36){
		batterylevel = BATTERY_CRITICAL;
		drawBatteryLow();
		_delay_ms(10000); // wait 10 secs
		disableDisplay();
	}
	else if(batteryVoltage < 4.76)
		batterylevel = BATTERY_LOW;
	else
		batterylevel = BATTERY_NORMAL;
	

	if (reset == 1 && initPressure != sensorPressure) reset = 0;

	// --------------- Display Debugging Info -------------------- //
	#ifdef DEBUG
	sprintf(displayADC, "ADC:= %u", sensorValue);
	int volt = (sensorVoltage - (int) sensorVoltage) * 100;
	sprintf(displayVoltage, "V:= %d.%d", (int) sensorVoltage, volt);
	sprintf(displayResistance, "R:= %lu", (unsigned long)resistance);

	if (reset == 1) {
		sprintf(displayPressure, "P: WRAP");
		sprintf(displayScalePressure, "P:= WRAP");
		} else if (sensorPressure > (65 + offset)) {
		sprintf(displayPressure, "P:= 65+");
		sprintf(displayScalePressure, "P:= 65+");
		} else {
		sprintf(displayPressure, "P:= %d", (int)sensorPressure);
		sprintf(displayScalePressure, "P:= %d", roundPressure());
	}

	// ------------- Production Display (Larger Numbers) --------- //
	#else
	if (sensorPressure > (65 + offset)){
		sprintf(displayPressure, "65+");
	}
	else{
		sprintf(displayPressure, "%d", roundPressure());

	// --------------------- Update LCD Screen ------------------- //
	}
	#endif
	
	u8g_FirstPage(&u8g);
	do {
		draw();
	} while (u8g_NextPage(&u8g));

}

void initBoard(void) {	
	 /* Configure LED, LCD, ADC I/O */
	 set_bit(DDRD, 2);    // Pin PD2 (ADC) is now configured as an OUTPUT
	 set_bit(DDRD, 3);    // Pin PD3 (LED) is now configured as an OUTPUT
	 set_bit(DDRD, 4);    // Pin PD4 (LCD) is now configured as an OUTPUT
	 set_bit(DDRD, 5);    // Pin PD5 (CS) is now configured as an OUTPUT
	 set_bit(DDRD, 7);    // Pin PD5 (A0) is now configured as an OUTPUT
	 set_bit(DDRD, 6);    // Pin PD5 (RST) is now configured as an OUTPUT
	 
	 /* Enable Display */
	 enableDisplay();
	 
	 /* Enable Sleep Timer 15 mins (set in timer.h*/
	 Timer_Init();
	 Timer_Enable();
	 
	 
	/*
    CS: PORTD, Bit 5 --> PN(3,5)
    A0: PORTD, Bit 7 --> PN(3,7)
    SCK: PORTB, Bit 1 --> PN(1,1)
    MOSI: PORTB, Bit 2 --> PN(1,2)
	RST: PORTD, Bit 6 --> PN(3,6)
    */
    u8g_InitSPI(&u8g, &u8g_dev_st7565_lm6059_sw_spi, PN(1, 1) /*SCK PB1*/, PN(1, 2) /*MOSI PB2*/, PN(3, 5) /*CS PD5*/, PN(3, 7) /*AO PD7*/, PN(3, 6) /* RST PD 6*/);
	
	u8g_SetColorIndex(&u8g, 1);		// assign default color value  	
	u8g_SetContrast(&u8g, 100);		// set the contrast
}

void enableDisplay(void) {
	 set_bit(PORTD, 2);   // sets the ADC on
	 set_bit(PORTD, 3);   // sets the LED on
	 set_bit(PORTD, 4);   // sets the LCD on
}

void disableDisplay(void) {
	clear_bit(PORTD, 2);   // sets the ADC off
	clear_bit(PORTD, 3);   // sets the LED off
	clear_bit(PORTD, 4);   // sets the LCD off
}

void draw(void) {
	// graphic commands to redraw the complete screen should be placed here
	
	if(batterylevel == BATTERY_LOW){
		drawBatteryLow();
	}

	if (reset == 1) {
		u8g_SetFont(&u8g, u8g_font_fub30r);
		u8g_DrawStr(&u8g, 5, 56, "WRAP");
		} else {

		// DEBUG
		#ifdef DEBUG
		u8g_SetFont(&u8g, u8g_font_unifont);
		u8g_DrawStr(&u8g, 0, 10,  displayADC);
		u8g_DrawStr(&u8g, 0, 22,  displayVoltage);
		u8g_DrawStr(&u8g, 0, 34,  displayResistance);
		u8g_DrawStr(&u8g, 0, 46,  displayPressure);
		u8g_DrawStr(&u8g, 0, 58,  displayScalePressure);

		//PRODUCTION
		#else
		u8g_SetFont(&u8g, u8g_font_fub49n);
		u8g_DrawStr(&u8g, 0, 56,  displayPressure);
		u8g_SetFont(&u8g, u8g_font_unifont);
		u8g_DrawStr(&u8g, 90, 62,  "MMHG");
		#endif
	}
}

void drawBatteryLow(void) {
	u8g_SetFont(&u8g, u8g_font_unifont);
	u8g_DrawStr(&u8g, 90, 10,  "BATT");
	clear_bit(PORTD, 3);   // sets the LED off
}


