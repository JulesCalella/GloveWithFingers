#ifndef DEVICE_CALLS_H
#define	DEVICE_CALLS_H

#include <xc.h>

// Defines used to reset the individual registers
#define CTRL_1_AG  0xC0
#define CTRL_2_AG  0x00
#define CTRL_3_AG  0x00
#define CTRL_4_AG  0x38
#define CTRL_5_AG  0x38
#define CTRL_6_AG  0xD0
#define CTRL_7_AG  0x60
#define CTRL_8_AG  0x04
#define CTRL_9_AG  0x04
#define CTRL_10_AG 0x00
#define CTRL_1_M   0x7E
#define CTRL_2_M   0x00
#define CTRL_3_M   0x84
#define CTRL_4_M   0x0C
#define CTRL_5_M   0x00

// =============================================================================
// |                            FUNCTIONS                                      |
// =============================================================================
// Functions for reading the finger bend
void readFinger1(char *rdyFlag, int *data);     //Thumb
void readFinger2(char *rdyFlag, int *data);     //Index Finger

// Functions for reading the accelerometer values
void readAccX(char *rdyFlag, int *data);
void readAccY(char *rdyFlag, int *data);
void readAccZ(char *rdyFlag, int *data);

// Functions for reading the gyroscope values
void readGyroX(char *rdyFlag, int *data);
void readGyroY(char *rdyFlag, int *data);
void readGyroZ(char *rdyFlag, int *data);

// Functions for reading the magnetometer values
void readMagX(char *rdyFlag, int *data);
void readMagY(char *rdyFlag, int *data);
void readMagZ(char *rdyFlag, int *data);

// Functions for activating the individual modules
char activateFinger(char on);
char activateAcc(char on);
char activateGyro(char on);
char activateMag(char on);

// Configurations 
char sensitivityAcc(char choice);
char sensitivityGyro(char choice);
char sensitivityMag(char choice);

// Control Registers
void control_1_AG(int value);
void control_2_AG(int value);
void control_3_AG(int value);
void control_4_AG(int value);
void control_5_AG(int value);
void control_6_AG(int value);
void control_7_AG(int value);
void control_8_AG(int value);
void control_9_AG(int value);
void control_10_AG(int value);
void control_1_M(int value);
void control_2_M(int value);
void control_3_M(int value);
void control_4_M(int value);
void control_5_M(int value);
void resetRegisters();


#endif

