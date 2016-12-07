
/* Target Ardufruit Metro */
#include <U8glib.h>
#include <math.h>

//#define DEBUG

/* Pressure Curve Fit 11-28-2016 */
#define P1 458.6    // 450
#define P2 -0.7116  // -0.7034
#define P3 4.604    // 4.284
#define offset 2.5

#define VREF 5.082 //5.0365
#define ADC_OFFSET 0
#define PRESSURE_OFFSET 0


int          sensorPin         = A0;    // select the sensor input pin
unsigned int sensorValue       = 0;     // variable to store the value coming from the sensor
float        sensorVoltage     = 0;
unsigned int sensorPressure    = 0;
float        resistance        = 0;
int          reset             = 1;
unsigned int initPressure      = 0;

char   displayADC[10];
char   displayPressure[10];
char   displayResistance[10];
char   displayVoltage[10];
char   displayScalePressure[10];

// pin 9 - Serial data out (SID)
// pin 8 - Serial clock out (SCLK)
// pin 7 - Data/Command select (RS or A0)
// pin 6 - LCD reset (RST)
// pin 5 - LCD chip select (CS)
U8GLIB_LM6059_2X u8g(8, 9, 5, 7, 6); //(sck, mosi, cs, a0 [, reset])

void draw(void) {
    // graphic commands to redraw the complete screen should be placed here

    if (reset == 1) {
        u8g.setFont(u8g_font_fub30r);
        u8g.drawStr(5, 56, "WRAP");
    } else {

        // DEBUG
#ifdef DEBUG
        u8g.setFont(u8g_font_unifont);
        u8g.drawStr(0, 10, displayADC);
        u8g.drawStr(0, 22, displayVoltage);
        u8g.drawStr(0, 34, displayResistance);
        u8g.drawStr(0, 46, displayPressure);
        u8g.drawStr(0, 58, displayScalePressure);

        //PRODUCTION
#else
        u8g.setFont(u8g_font_fub49n);
        u8g.drawStr(0, 56, displayPressure);
        u8g.setFont(u8g_font_unifont);
        u8g.drawStr(90, 62, "MMHG");

#endif
    }
}

void setup(void) {
    // flip screen, if required
    // u8g.setRot180();

    // set SPI backup if required
    //u8g.setHardwareBackup(u8g_backup_avr_spi);

    // assign default color value
    if (u8g.getMode() == U8G_MODE_R3G3B2) {
        u8g.setColorIndex(255);       // white
    } else if (u8g.getMode() == U8G_MODE_GRAY2BIT) {
        u8g.setColorIndex(3);         // max intensity
    } else if (u8g.getMode() == U8G_MODE_BW) {
        u8g.setColorIndex(1);         // pixel on
    } else if (u8g.getMode() == U8G_MODE_HICOLOR) {
        u8g.setHiColorByRGB(255, 255, 255);
    }

    // set the contrast
    u8g.setContrast(100); //110

    analogReference(DEFAULT); // DEFAULT, INTERNAL, INTERNAL1V1, INTERNAL2V56, or EXTERNAL

    // get initial voltage
    calcPressure();
    initPressure = sensorPressure;
}

void calcVoltage() {
    // read the value from the sensor:
    sensorValue = analogRead(sensorPin) -  ADC_OFFSET;
    if (sensorValue < 0) sensorValue = 0;

    sensorVoltage = sensorValue * VREF / 1023.0;
}

void calcPressure() {
    // read the voltage for ADC
    calcVoltage();

    // compute the resistance from voltage
    resistance = (27.25 * sensorVoltage + 64.015);    // MOhms
    resistance /= (12803.0 - 2650.0 * sensorVoltage); // Ohms
    resistance *= 1000000;


    // compute pressure from resistance
    sensorPressure =  P1 * pow(resistance / 1000.0, P2) + P3;
    sensorPressure -= PRESSURE_OFFSET;

    // max of pressure
    if (sensorPressure >= 99) sensorPressure = 99;

    if (sensorPressure <= 20) reset = 1;
}


int roundPressure() {
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


int floorPressure() {
    int retval = 0;
    if (sensorPressure >= 10 && sensorPressure < 15) retval = 10;
    else if (sensorPressure >= 15 && sensorPressure < 20) retval = 15;
    else if (sensorPressure >= 20 && sensorPressure < 25) retval = 20;
    else if (sensorPressure >= 25 && sensorPressure < 30) retval = 25;
    else if (sensorPressure >= 30 && sensorPressure < 35) retval = 30;
    else if (sensorPressure >= 35 && sensorPressure < 40) retval = 35;
    else if (sensorPressure >= 40 && sensorPressure < 45) retval = 40;
    else if (sensorPressure >= 45 && sensorPressure < 50) retval = 45;
    else if (sensorPressure >= 50 && sensorPressure < 55) retval = 50;
    else if (sensorPressure >= 55 && sensorPressure < 60) retval = 55;
    else if (sensorPressure >= 60 && sensorPressure < 65) retval = 60;
    else if (sensorPressure >= 65 && sensorPressure < 100) retval = 65;
    else retval = 5;

    return retval;
}


void loop(void) {
    // -------------- read the sensors voltage ------------------- //
    calcPressure();

    if (reset == 1 && initPressure != sensorPressure) reset = 0;

    // --------------- Display Debugging Info -------------------- //
#ifdef DEBUG
    sprintf(displayADC, "ADC:= %u", sensorValue);
    int volt = (sensorVoltage - (int)sensorVoltage) * 100;
    sprintf(displayVoltage, "V:= %d.%d", (int)sensorVoltage, volt);
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
        //sprintf(displayScalePressure, "P:= %d", (int) initPressure);
    }

    // ------------- Production Display (Larger Numbers) --------- //
#else
    if (sensorPressure > (65 + offset)) sprintf(displayPressure, "65+");
    else sprintf(displayPressure, "%d", /*sensorPressure*/ roundPressure());
#endif

    // --------------------- Update LCD Screen ------------------- //
    u8g.firstPage();
    do {
        draw();
    } while (u8g.nextPage());

    // ------------ rebuild the picture after some delay ---------- //
    delay(200);
}




