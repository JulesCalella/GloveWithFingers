#include <xc.h>
#include "deviceCalls.h"
#include "config.h"
#include "spiComm.h"

/*******************************************************************************
 * 'deviceCalls' - containts all the functions required to retieve data from 
 * the peripherals, as well as alter their properties.
 * 
 *******************************************************************************/

// =============================================================================
// |                             Global Variables                              |
// =============================================================================

char statusXL = 0x00;   // 0, z, y, x
char statusG = 0x00;    // 0, z, y, x
char statusM = 0x00;    // 0, z, y, x
char statusF = 0x00;

int ctrl1_ag =  CTRL_1_AG;  // 110 00 0 00
int ctrl2_ag =  CTRL_2_AG;  // 0000 00 00
int ctrl3_ag =  CTRL_3_AG;  // 0 0 00 0000
int ctrl4_ag =  CTRL_4_AG;  // 00 111 0 0 0
int ctrl5_ag =  CTRL_5_AG;  // 00 1 1 1 000
int ctrl6_ag =  CTRL_6_AG;  // 110 10 0 00
int ctrl7_ag =  CTRL_7_AG;  // 0 11 00 0 0 0
int ctrl8_ag =  CTRL_8_AG;  // 0 0 0 0 0 1 0 0
int ctrl9_ag =  CTRL_9_AG;  // 0 0 0 0 0 1 0 0
int ctrl10_ag = CTRL_10_AG; // 00000 0 0 0

int ctrl1_m = CTRL_1_M;  // 0 11 111 1 0
int ctrl2_m = CTRL_2_M;  // 0 00 0 0 0 00
int ctrl3_m = CTRL_3_M;  // 1 0 0 00 1? 00
int ctrl4_m = CTRL_4_M;  // 0000 11 0 0
int ctrl5_m = CTRL_5_M;  // 0 0 000000

// =============================================================================
// |                            READ FINGER BEND                               |
// =============================================================================
// 'readFinger1()' - reads the value of the flex sensor on the thumb and 
// returns the value as an integer.  If the module is off, the function 
// sets the rdyFlag to 0. 
// =============================================================================
void readFinger1(char *rdyFlag, int *data)
{
    *rdyFlag = 0;
    
    if(AD1CON1bits.ADON == 1)
    {        
        int wait = 0;

        *rdyFlag = 1;
        
        // Begin sampling
        AD1CON1bits.SAMP = 1;

        // Wait until sampling is complete
        while(wait < 50) wait++;

        // End sampling 
        AD1CON1bits.SAMP = 0;

        // Wait until conversion is complete
        while(!AD1CON1bits.DONE) continue;

        // Retrieve value
        *data = ADC1BUF0;
    }
}

// =============================================================================
// 'readFinger2()' - reads the value of the flex sensor on the index finger 
// and returns the value as an integer. If the module is off, the function 
// sets the rdyFlag to 0.
// =============================================================================
void readFinger2(char *rdyFlag, int *data)
{
    *rdyFlag = 0;
    
    if(AD1CON1bits.ADON == 1)
    {
        int wait = 0;
        
        *rdyFlag = 1;

        // Begin sampling
        AD1CON1bits.SAMP = 1;

        // Wait until sampling is complete
        while(wait < 50) wait++;

        // End sampling 
        AD1CON1bits.SAMP = 0;

        // Wait until conversion is complete
        while(!AD1CON1bits.DONE) continue;

        // Retrieve value
        *data = ADC1BUF0;
    }
}


// =============================================================================
// |                            ACCELEROMETER                                  |
// =============================================================================
// 'readAccX()' - reads the data from the accelerometer's x-axis and returns
//  the value. The function sets the rdyFlag to 0 if the device is not on 
// =============================================================================
void readAccX(char *rdyFlag, int *data)
{
    *rdyFlag = 0;
    
    if((statusXL & 0x01) != 0)
    {
        // Device is on, read the data
        *rdyFlag = 1;
        //*data = value from accelerometer
        SS_AG = 0;
        *data = SPIRead(0x29);
        SS_AG = 1;
        
        SS_AG = 0;
        *data = (*data << 8) | SPIRead(0x28);
        SS_AG = 1;
    }
}

// =============================================================================
// 'readAccY()' - reads the data from the accelerometer's y-axis and returns
// the value.  The function sets the rdyFlag to 0 if the device is not on
// =============================================================================
void readAccY(char *rdyFlag, int *data)
{
    *rdyFlag = 0;
    
    if((statusXL & 0x02) != 0)
    {
        // Device is on, read the data
        *rdyFlag = 1;
        //*data = value from accelerometer
        SS_AG = 0;
        *data = SPIRead(0x2B);
        SS_AG = 1;
        
        SS_AG = 0;
        *data = (*data << 8) | SPIRead(0x2A);
        SS_AG = 1;
    }
}

// =============================================================================
// 'readAccZ()' - reads the data from the accelerometer's z-axis and returns
// the value. The function sets the rdyFlag to 0 if the device is not on
// =============================================================================
void readAccZ(char *rdyFlag, int *data)
{
    *rdyFlag = 0;
    
    if((statusXL & 0x04) != 0)
    {
        // Device is on, read the data
        *rdyFlag = 1;
        //*data = value from accelerometer
        SS_AG = 0;
        *data = SPIRead(0x2D);
        SS_AG = 1;
        
        SS_AG = 0;
        *data = (*data << 8) | SPIRead(0x2C);
        SS_AG = 1;
    }
}

// =============================================================================
// |                                GYROSCOPE                                  |
// =============================================================================
// 'readGyroX()' - reads the data from the gyroscope's x-axis and returns 
// the value. The function sets the rdyFlag to 0 if the device is not on
 // ============================================================================
void readGyroX(char *rdyFlag, int *data)
{
    *rdyFlag = 0;
    
    if((statusG & 0x01) != 0)
    {
        // Device is on, read the data
        *rdyFlag = 1;
        //*data = value from gyroscope
        SS_AG = 0;
        *data = SPIRead(0x19);
        SS_AG = 1;
        
        SS_AG = 0;
        *data = (*data << 8) | SPIRead(0x18);
        SS_AG = 1;
    }
}


// =============================================================================
// 'readGyroY()' - reads the data from the gyroscope's y-axis and returns 
// the value. The function sets the rdyFlag to 0 if the device is not on
// =============================================================================
void readGyroY(char *rdyFlag, int *data)
{
    *rdyFlag = 0;
    
    if((statusG & 0x02) != 0)
    {
        // Device is on, read the data
        *rdyFlag = 1;
        //*data = value from gyroscope
        SS_AG = 0;
        *data = SPIRead(0x1B);
        SS_AG = 1;
        
        SS_AG = 0;
        *data = (*data << 8) | SPIRead(0x1A);
        SS_AG = 1;
    }
}

// =============================================================================
// 'readGyroZ()' - reads the data from the gyroscope's z-axis and returns 
// the value. The function sets the rdyFlag to 0 if the device is not on
// =============================================================================
void readGyroZ(char *rdyFlag, int *data)
{
    *rdyFlag = 0;
    
    if((statusG & 0x04) != 0)
    {
        // Device is on, read the data
        *rdyFlag = 1;
        //*data = value from gyroscope
        SS_AG = 0;
        *data = SPIRead(0x1D);
        SS_AG = 1;
        
        SS_AG = 0;
        *data = (*data << 8) | SPIRead(0x1C);
        SS_AG = 1;
    }
}


// =============================================================================
// |                                MAGNETOMETER                               |
// =============================================================================
// 'readMagX()' - reads the data from the magnetometer's x-axis and returns
// the value. The function sets the rdyFlag to 0 if the device is not on
// =============================================================================
void readMagX(char *rdyFlag, int *data)
{
    *rdyFlag = 0;
    
    if((statusM & 0x01) != 0)
    {
        // Device is on, read the data
        *rdyFlag = 1;
        
        SS_M = 0;
        *data = SPIRead(0x29);
        SS_M = 1;

        SS_M = 0;
        *data = (*data << 8) | SPIRead(0x28);
        SS_M = 1;
    }
}

// =============================================================================
// 'readMagY()' - reads the data from the magnetometer's y-axis and returns
// the value. The function sets the rdyFlag to 0 if the device is not on
// =============================================================================
void readMagY(char *rdyFlag, int *data)
{
    *rdyFlag = 0;
    
    if((statusM & 0x02) != 0)
    {
        // Device is on, read the data
        *rdyFlag = 1;
        
        SS_M = 0;
        *data = SPIRead(0x2B);
        SS_M = 1;

        SS_M = 0;
        *data = (*data << 8) | SPIRead(0x2A);
        SS_M = 1;
    }
}

// =============================================================================
// 'readMagZ()' - reads the data from the magnetometer's z-axis and returns
// the value. The function sets the rdyFlag to 0 if the device is not on
// =============================================================================
void readMagZ(char *rdyFlag, int *data)
{
    *rdyFlag = 0;
    
    if((statusM & 0x04) != 0)
    {
        // Device is on, read the data
        *rdyFlag = 1;
        
        SS_M = 0;
        *data = SPIRead(0x2D);
        SS_M = 1;

        SS_M = 0;
        *data = (*data << 8) | SPIRead(0x2C);
        SS_M = 1;
    }
}


// =============================================================================
// |                               ACTIVATE                                    |
// =============================================================================
// 'activateFinger(char on)' - takes a char value and turns on the ADC modules 
// if it is 1 and turns it off if it is 0. The function returns 0 upon success, 
// or -1 if an invalid entry was made. 
// =============================================================================
char activateFinger(char on)
{
    if(on == 0){
        AD1CON1bits.ADON = 0;
        statusF = 0x00;
    }else if(on == 1){
        AD1CON1bits.ADON = 1;
        statusF = 0x01;
    }else{
        return -1;
    }
    
    return 0;
}

// =============================================================================
// 'activateAcc(char on)' - takes a char value and turns the accelerometer on 
// if it is 1 and turns it off if it is 0. The function returns 0 upon success, 
// or -1 if an invalid entry was made. When the accelerometer is turned on,
// it automatically sets the ODR to 952 Hz, the highest available setting.
// =============================================================================
char activateAcc(char on)
{
    if(on == 1){
        ctrl6_ag |= 0xE0;   // 1110 0000
        
        SS_AG = 0;
        SPIWrite(0x20, ctrl6_ag);
        SS_AG = 1;
        
        statusXL = 0x07;
    }else if(on == 0){
        ctrl6_ag &= 0x1F;   // 0001 1111
        
        SS_AG = 0;
        SPIWrite(0x20, ctrl6_ag);
        SS_AG = 1;
        
        statusXL = 0x00;
    }else{
        return -1;
    }
    
    return 0;
}

// =============================================================================
// 'activateGyro(char on)' - takes a char value and turns the gyroscope on 
// if it is 1 and turns it off if it is 0. The function returns 0 upon success, 
// or -1 if an invalid entry was made. When the gyroscope is turned on,
// it automatically sets the ODR to 952 Hz, the highest available setting.
// =============================================================================
char activateGyro(char on)
{
    if(on == 1){
        ctrl1_ag |= 0xE0;
        
        SS_AG = 0;
        SPIWrite(0x10, ctrl1_ag);
        SS_AG = 1;
        
        statusG = 0x07;
    }else if(on == 0){
        ctrl1_ag &= 0x07;
        
        SS_AG = 0;
        SPIWrite(0x10, ctrl1_ag);
        SS_AG = 1;
        
        statusG = 0x00;
    }else{
        return -1;
    }
    
    return 0;
}

// =============================================================================
// 'activateMag(char on)' - takes a char value and turns the magnetometer on 
// if it is 1 and turns it off if it is 0. The function returns 0 upon success, 
// or -1 if an invalid entry was made. 
// =============================================================================
char activateMag(char on)
{
    if(on == 1){
        // TODO: Turn on the device
        ctrl3_m |= 0x03;
        
        SS_M = 0;
        SPIWrite(0x22, ctrl3_m);
        SS_M = 1;
        
        statusM = 0x07;
    }else if(on == 0){
        // TODO: Turn off the device
        ctrl3_m &= 0xFC;
        
        SS_M = 0;
        SPIWrite(0x22, ctrl3_m);
        SS_M = 1;
        
        statusM = 0x00;
    }else{
        return -1;
    }
    
    return 0;
}

// *****************************************************************************
//                                  CONFIGURE

// =============================================================================
// |                            ACCELEROEMETER                                 |
// =============================================================================
// 'sensitivityAcc(char choice)' - takes a value from 0 to 4 and configures the
// the sensitivity of the readings. (0) 2g, (1) 16g, (2) 4g, (3) 8g 
// =============================================================================
char sensitivityAcc(char choice)
{
    if((choice > 3) || (choice < 0)) return 1; // Invalid choice
    
    ctrl6_ag &= 0xE7;   //11100111
    ctrl6_ag |= (choice << 3);
    
    SS_AG = 0;
    SPIWrite(0x20, ctrl6_ag);
    SS_AG = 1;
    
    return 0;   // Return 0 upon success
}

// =============================================================================
// |                                 GYROSCOPE                                 |
// =============================================================================
// 'sensitivityGyro(char choice)' - takes a value from 0 to 4 and configures the
// the sensitivity of the readings. (0) 245 dps, (1) 500 dps, (2) 2000 dps 
// =============================================================================
char sensitivityGyro(char choice)
{
    if((choice > 2) || (choice < 0)) return 1; // Invalid choice
    
    if(choice == 2) choice = 3;
    
    ctrl1_ag &= 0xE7;   //11100111
    ctrl1_ag |= (choice << 3);
    
    SS_AG = 0;
    SPIWrite(0x10, ctrl1_ag);
    SS_AG = 1;
    
    return 0;   // Return 0 upon success
}

// =============================================================================
// |                              MAGNETOMETER                                 |
// =============================================================================
// 'sensitivityMag(char choice)' - takes a value from 0 to 4 and configures the
// the sensitivity of the readings. (0) 4 gauss, (1) 8 gauss, (2) 12 gauss, 
// (3) 16 gauss 
// =============================================================================
char sensitivityMag(char choice)
{
    if((choice > 3) || (choice < 0)) return 1; // Invalid choice
    
    ctrl2_m &= 0x9F;   //10011111
    ctrl2_m |= (choice << 5);
    
    SS_M = 0;
    SPIWrite(0x21, ctrl2_ag);
    SS_M = 1;
    
    return 0;   // Return 0 upon success
}

// =============================================================================
// |                         ++ CONTROL REGISTERS ++                           |
// =============================================================================
// |                       ACCELEROEMTER AND GYROSCOPE                         |
// =============================================================================
// 'control_1_AG(int value)' - uses the value passed to rewrite the value stored
// in ctrl1_ag and sends it to the 9-axis sensor.
// =============================================================================
void control_1_AG(int value)
{
    ctrl1_ag = value;
    
    SS_AG = 0;
    SPIWrite(0x10, ctrl1_ag);
    SS_AG = 1;
}

// =============================================================================
// 'control_2_AG(int value)' - uses the value passed to rewrite the value stored
// in ctrl2_ag and sends it to the 9-axis sensor.
// =============================================================================
void control_2_AG(int value)
{
    ctrl2_ag = value;
    
    SS_AG = 0;
    SPIWrite(0x11, ctrl2_ag);
    SS_AG = 1;
}

// =============================================================================
// 'control_4_AG(int value)' - uses the value passed to rewrite the value stored
// in ctrl3_ag and sends it to the 9-axis sensor.
// =============================================================================
void control_3_AG(int value)
{
    ctrl3_ag = value;
    
    SS_AG = 0;
    SPIWrite(0x12, ctrl3_ag);
    SS_AG = 1;
}

// =============================================================================
// 'control_4_AG(int value)' - uses the value passed to rewrite the value stored
// in ctrl4_ag and sends it to the 9-axis sensor.
// =============================================================================
void control_4_AG(int value)
{
    ctrl4_ag = value;
    
    SS_AG = 0;
    SPIWrite(0x1E, ctrl4_ag);
    SS_AG = 1;
}

// =============================================================================
// 'control_5_AG(int value)' - uses the value passed to rewrite the value stored
// in ctrl5_ag and sends it to the 9-axis sensor.
// =============================================================================
void control_5_AG(int value)
{
    ctrl5_ag = value;
    
    SS_AG = 0;
    SPIWrite(0x1F, ctrl5_ag);
    SS_AG = 1;
}

// =============================================================================
// 'control_6_AG(int value)' - uses the value passed to rewrite the value stored
// in ctrl6_ag and sends it to the 9-axis sensor.
// =============================================================================
void control_6_AG(int value)
{
    ctrl6_ag = value;
    
    SS_AG = 0;
    SPIWrite(0x20, ctrl6_ag);
    SS_AG = 1;
}

// =============================================================================
// 'control_7_AG(int value)' - uses the value passed to rewrite the value stored
// in ctrl7_ag and sends it to the 9-axis sensor.
// =============================================================================
void control_7_AG(int value)
{
    ctrl7_ag = value;
    
    SS_AG = 0;
    SPIWrite(0x21, ctrl7_ag);
    SS_AG = 1;
}

// =============================================================================
// 'control_8_AG(int value)' - uses the value passed to rewrite the value stored
// in ctrl8_ag and sends it to the 9-axis sensor.
// =============================================================================
void control_8_AG(int value)
{
    ctrl8_ag = value;
    
    SS_AG = 0;
    SPIWrite(0x22, ctrl8_ag);
    SS_AG = 1;
}

// =============================================================================
// 'control_9_AG(int value)' - uses the value passed to rewrite the value stored
// in ctrl9_ag and sends it to the 9-axis sensor.
// =============================================================================
void control_9_AG(int value)
{
    ctrl9_ag = value;
    
    SS_AG = 0;
    SPIWrite(0x23, ctrl9_ag);
    SS_AG = 1;
}

// =============================================================================
// 'control_10_AG(int value)' - uses the value passed to rewrite the value
// stored in ctrl10_ag and sends it to the 9-axis sensor.
// =============================================================================
void control_10_AG(int value)
{
    ctrl10_ag = value;
    
    SS_AG = 0;
    SPIWrite(0x24, ctrl10_ag);
    SS_AG = 1;
}

// =============================================================================
// |                            MAGNETOMETER                                   |
// =============================================================================
// 'control_1_M(int value)' - uses the value passed to rewrite the value stored
// in ctrl1_m and sends it to the 9-axis sensor.
// =============================================================================
void control_1_M(int value)
{
    ctrl1_m = value;
    
    SS_M = 0;
    SPIWrite(0x20, ctrl1_m);
    SS_M = 1;
}

// =============================================================================
// 'control_2_M(int value)' - uses the value passed to rewrite the value stored
// in ctrl2_m and sends it to the 9-axis sensor.
// =============================================================================
void control_2_M(int value)
{
    ctrl2_m = value;
    
    SS_M = 0;
    SPIWrite(0x21, ctrl2_m);
    SS_M = 1;
}   

// =============================================================================
// 'control_3_M(int value)' - uses the value passed to rewrite the value stored
// in ctrl3_m and sends it to the 9-axis sensor.
// =============================================================================
void control_3_M(int value)
{
    ctrl3_m = value;
    
    SS_M = 0;
    SPIWrite(0x22, ctrl3_m);
    SS_M = 1;
}

// =============================================================================
// 'control_4_M(int value)' - uses the value passed to rewrite the value stored
// in ctrl4_m and sends it to the 9-axis sensor.
// =============================================================================
void control_4_M(int value)
{
    ctrl4_m = value;
    
    SS_M = 0;
    SPIWrite(0x23, ctrl4_m);
    SS_M = 1;
}

// =============================================================================
// 'control_5_M(int value)' - uses the value passed to rewrite the value stored
// in ctrl5_m and sends it to the 9-axis sensor.
// =============================================================================
void control_5_M(int value)
{
    ctrl5_m = value;
    
    SS_M = 0;
    SPIWrite(0x24, ctrl5_m);
    SS_M = 1;
}

// =============================================================================
// 'resetRegisters()' - resets all the registers to default values. Also used 
// in initial setup to write to all registers. 
// =============================================================================
void resetRegisters()
{
    control_1_AG(CTRL_1_AG);
    control_2_AG(CTRL_2_AG);
    control_3_AG(CTRL_3_AG);
    control_4_AG(CTRL_4_AG);
    control_5_AG(CTRL_5_AG);
    control_6_AG(CTRL_6_AG);
    control_7_AG(CTRL_7_AG);
    control_8_AG(CTRL_8_AG);
    control_9_AG(CTRL_9_AG);
    control_10_AG(CTRL_10_AG);
    
    control_1_M(CTRL_1_M);
    control_2_M(CTRL_2_M);
    control_3_M(CTRL_3_M);
    control_4_M(CTRL_4_M);
    control_5_M(CTRL_5_M);
}

// *****************************************************************************
//                                   STATUS                                     
 
