#ifndef CONFIGURATION_H
#define	CONFIGURATION_H

#include <xc.h> 

#define SS_AG LATBbits.LATB0      // Slave Select for Accelerometer and Gyroscope
#define SS_M LATBbits.LATB0       // Slave Select for Magnetometer

void oscInit();
void adcInit();

#endif	