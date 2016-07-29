#include <xc.h>
#include "config.h"
#include "spiComm.h"
#include "deviceCalls.h"

//-------------------------------------------------------------
// TEMP defines for displaying values
#define LED1() {LATBbits.LATB1 = 1; LATBbits.LATB0 = 1;}    //|
#define LED2() {LATBbits.LATB1 = 1; LATBbits.LATB0 = 1;}    //|
#define LED3() {LATBbits.LATB1 = 1; LATBbits.LATB0 = 1;}    //|
#define LED4() {LATBbits.LATB1 = 1; LATBbits.LATB0 = 1;}    //|
#define LED5() {LATBbits.LATB1 = 1; LATBbits.LATB0 = 1;}    //|
#define LED6() {LATBbits.LATB1 = 1; LATBbits.LATB0 = 1;}    //|
#define LED7() {LATBbits.LATB1 = 1; LATBbits.LATB0 = 1;}    //|
#define LED8() {LATBbits.LATB1 = 1; LATBbits.LATB0 = 1;}    //|
#define LED9() {LATBbits.LATB1 = 1; LATBbits.LATB0 = 1;}    //|
                                                            //|
#define LED1_ON() {LATB = 0x0000; LED1();}                  //|
#define LED2_ON() {LATB = 0x0000; LED2();}                  //|
#define LED3_ON() {LATB = 0x0000; LED3();}                  //|
#define LED4_ON() {LATB = 0x0000; LED4();}                  //|
#define LED5_ON() {LATB = 0x0000; LED5();}                  //|
#define LED6_ON() {LATB = 0x0000; LED6();}                  //|
#define LED7_ON() {LATB = 0x0000; LED7();}                  //|
#define LED8_ON() {LATB = 0x0000; LED8();}                  //|
#define LED9_ON() {LATB = 0x0000; LED9();}                  //|
//-------------------------------------------------------------

// CONFIG4
#pragma config DSWDTPS = DSWDTPS1F      // Deep Sleep Watchdog Timer Postscale Select bits (1:68719476736 (25.7 Days))
#pragma config DSWDTOSC = LPRC          // DSWDT Reference Clock Select (DSWDT uses LPRC as reference clock)
#pragma config DSBOREN = OFF            // Deep Sleep BOR Enable bit (DSBOR Disabled)
#pragma config DSWDTEN = OFF            // Deep Sleep Watchdog Timer Enable (DSWDT Disabled)
#pragma config DSSWEN = OFF             // DSEN Bit Enable (Deep Sleep operation is always disabled)
#pragma config PLLDIV = DISABLED        // USB 96 MHz PLL Prescaler Select bits (PLL Disabled)
#pragma config I2C1SEL = DISABLE        // Alternate I2C1 enable bit (I2C1 uses SCL1 and SDA1 pins)
#pragma config IOL1WAY = OFF            // PPS IOLOCK Set Only Once Enable bit (The IOLOCK bit can be set and cleared using the unlock sequence)

// CONFIG3
#pragma config WPFP = WPFP127           // Write Protection Flash Page Segment Boundary (Page 127 (0x1FC00))
#pragma config SOSCSEL = OFF            // SOSC Selection bits (Digital (SCLKI) mode)
#pragma config WDTWIN = PS25_0          // Window Mode Watchdog Timer Window Width Select (Watch Dog Timer Window Width is 25 percent)
#pragma config PLLSS = PLL_FRC          // PLL Secondary Selection Configuration bit (PLL is fed by the on-chip Fast RC (FRC) oscillator)
#pragma config BOREN = OFF              // Brown-out Reset Enable (Brown-out Reset Disabled)
#pragma config WPDIS = WPDIS            // Segment Write Protection Disable (Disabled)
#pragma config WPCFG = WPCFGDIS         // Write Protect Configuration Page Select (Disabled)
#pragma config WPEND = WPENDMEM         // Segment Write Protection End Page Select (Write Protect from WPFP to the last page of memory)

// CONFIG2
#pragma config POSCMD = NONE            // Primary Oscillator Select (Primary Oscillator Disabled)
#pragma config WDTCLK = LPRC            // WDT Clock Source Select bits (WDT uses LPRC)
#pragma config OSCIOFCN = OFF           // OSCO Pin Configuration (OSCO/CLKO/RA3 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD           // Clock Switching and Fail-Safe Clock Monitor Configuration bits (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = FRCPLL           // Initial Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))
#pragma config ALTRB6 = RETAIN          // Alternate RB6 pin function enable bit (Keep the RP6/ASCL1/PMPD6 functions to RB6)
#pragma config ALTCMPI = CxINC_RB       // Alternate Comparator Input bit (C1INC is on RB13, C2INC is on RB9 and C3INC is on RA0)
#pragma config WDTCMX = WDTCLK          // WDT Clock Source Select bits (WDT clock source is determined by the WDTCLK Configuration bits)
#pragma config IESO = ON                // Internal External Switchover (Enabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler Select (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler Ratio Select (1:128)
#pragma config WINDIS = OFF             // Windowed WDT Disable (Standard Watchdog Timer)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT disabled in hardware; SWDTEN bit disabled)
#pragma config ICS = PGx1               // Emulator Pin Placement Select bits (Emulator functions are shared with PGEC1/PGED1)
#pragma config LPCFG = OFF              // Low power regulator control (Disabled - regardless of RETEN)
#pragma config GWRP = OFF               // General Segment Write Protect (Write to program memory allowed)
#pragma config GCP = OFF                // General Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (Disabled)

void ledDisplay(int data);

/* ADC Interrupt 
void __attribute__ ((__interrupt__)) _ADC1Interrupt(void)
{
    IFS0bits.AD1IF = 0;
}*/

// =============================================================================
// =                                MAIN                                       =
// =============================================================================
int main()
{
    int data;       // Used to hold the data from the READ functions
    char rdyFlag;   // Used to determine if devices are ready to be read from
    
    // Initial pin configurations
    TRISA = 0xFFFF;
    TRISB = 0xFFFF;
    LATA = 0x0000;
    LATB = 0x0000;
    
    // Device initialization
    oscInit();
    adcInit();
    spiPinInit();
    spiInit();
    
    // Turn on individual sensors
    activateFinger(1);
    activateAcc(1);
    activateGyro(1);
    activateMag(1);
    
    // Write initial values to registers
    resetRegisters();
    
    // Adjust sensitivity of reported values
    sensitivityAcc(2);  // 4g
    sensitivityGyro(0); // 245 dps
    sensitivityMag(0);  // 4 gauss
    
    //---------------------------------------------
    // TEMP Values for displaying without USB   //|
    int acc_x, acc_y, acc_z;                    //|
    int gyro_x, gyro_y, gyro_z;                 //|
    int mag_x, mag_y, mag_z;                    //|
    int finger1, finger2;                       //|
    //---------------------------------------------
    
    while(1)
    {
        readFinger1(&rdyFlag, &data);
            finger1 = data; // TEMP
            ledDisplay(data);           // LED DISPLAY
        readFinger2(&rdyFlag, &data);
            finger2 = data; // TEMP
        
        readAccX(&rdyFlag, &data);
            acc_x = data;   // TEMP
        readAccY(&rdyFlag, &data);
            acc_y = data;   // TEMP
        readAccZ(&rdyFlag, &data);
            acc_z = data;   // TEMP
        
        readGyroX(&rdyFlag, &data);
            gyro_x = data;  // TEMP
        readGyroY(&rdyFlag, &data);
            gyro_y = data;  // TEMP
        readGyroZ(&rdyFlag, &data);
            gyro_z = data;  // TEMP
        
        readMagX(&rdyFlag, &data);
            mag_x = data;   // TEMP
        readMagY(&rdyFlag, &data);
            mag_y = data;   // TEMP
        readMagZ(&rdyFlag, &data);
            mag_z = data;   // TEMP
        
    }
    
    return 0;
}

void ledDisplay(int data)
{
    int i = 0;
   
    for(i = 0; i < 10000; i++)
    {
        if( ((data & 0x0001) != 0) && (i%9 == 0) ) LED1_ON();
        if( ((data & 0x0002) != 0) && (i%9 == 1) ) LED2_ON();
        if( ((data & 0x0004) != 0) && (i%9 == 2) ) LED3_ON();
        if( ((data & 0x0008) != 0) && (i%9 == 3) ) LED4_ON();
        if( ((data & 0x0010) != 0) && (i%9 == 4) ) LED5_ON();
        if( ((data & 0x0020) != 0) && (i%9 == 5) ) LED6_ON();
        if( ((data & 0x0040) != 0) && (i%9 == 6) ) LED7_ON();
        if( ((data & 0x0080) != 0) && (i%9 == 7) ) LED8_ON();
        if( ((data & 0x0100) != 0) && (i%9 == 8) ) LED9_ON();  
    }
}