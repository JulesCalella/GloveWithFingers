#ifndef SPI_COMM_H
#define	SPI_COMM_H

#include <xc.h>

void spiInit();
void SPIWrite(int address, int data);
int SPIRead(int address);
void spiPinInit();

#endif	