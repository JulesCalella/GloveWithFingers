#include <xc.h>
#include "spiComm.h"

/*******************************************************************************
 * 
 * 'spiComm' is used to initialize the PIC24FJ128GB202 SPI module, read data 
 * from an SPI device, and write data to an SPI device.
 * 
 * Available Functions:
 *  > void SPIInit()
 *  > void SPIWrite(int address, int data)
 *  > int SPIRead(int address)
 * 
 ******************************************************************************/



// =============================================================================
//
//                              SPI INITIALIZE
//
// =============================================================================
void spiInit(){
    /*Disable Interrupts*/
    //IEC0bits.SPI1IE = 0;        // SPI1 General Interrupt Disabled
    //IEC0bits.SPI1TXIE = 0;      // Transmit complete interrupt Disabled
    //IEC3bits.SPI1RXIE = 0;      // Receive complete interrupt Disabled
    //IFS3bits.SPI1RXIF = 0;
    //IFS3bits.SPI1RXIF = 0;
    
    //INTCON1bits.NSTDIS = 1;
    
    IPC2bits.SPI1IP = 5;      //Priority is highest (7)
    
    IFS0bits.SPI1IF = 0;
    IFS0bits.SPI1TXIF = 0;
    IFS3bits.SPI1RXIF = 0;
    
    IEC0bits.SPI1IE = 0;        // SPI1 General Interrupt Disabled
    IEC0bits.SPI1TXIE = 0;      // Transmit complete interrupt Disabled
    IEC3bits.SPI1RXIE = 0;      // Receive complete interrupt Disabled
   
    
    /*Clear ON bit*/
    SPI1CON1Lbits.SPIEN = 0;
    
    /*Clear receiver buffer*/
    int clr = SPI1BUFL;
    
    /*Clear enhanced mode bit*/
    SPI1CON1Lbits.ENHBUF = 0;
        
    /*Clear overflow bit*/
    SPI1STATLbits.SPIROV = 0;
    
    /*Set master enable*/
    SPI1CON1Lbits.MSTEN = 1;
    
    /*Other CON settings*/
    SPI1BRGL = 0xFFFF;                //FPB/(2*(SPI1BRGL +1))   7999
    SPI1CON1Lbits.MODE = 0x0;       //8-bit data
    SPI1CON1Lbits.DISSDO = 0;       //SDO1 controlled by module
    SPI1CON1Lbits.DISSDI = 0;       //SDI1 controlled by module
    SPI1CON1Lbits.DISSCK = 0;       //SCK1 is controlled by module
    SPI1CON1Lbits.CKP = 1;          //Clock idle is low, active high
    SPI1CON1Lbits.CKE = 0;          //Transmit from idle to active   ___-_-_-_-_-_-_-_-___
    SPI1CON1Lbits.SMP = 0;          //Input sampled at end of data output time
    
    SPI1CON1Hbits.AUDEN = 0;        //Audio protocol is disabled
    SPI1CON1Hbits.IGNROV = 1;       //Ignore overflow error
    SPI1CON1Hbits.IGNTUR = 1;       //Ignore transmit underrun error
    SPI1CON1Hbits.MSSEN = 0;        //Slave select is controlled by I/O
    SPI1CON2Lbits.WLENGTH = 0x00;   //Use SPI1CON1Lbits.MODE
    
    //SPI1IMSKLbits.BUSYEN = 1;
    //SPI1IMSKLbits.SPIRBFEN = 1;
   
    /*Set on bit*/
    SPI1CON1Lbits.SPIEN = 1;
    
} 

// =============================================================================
//
//                                  SPI WRITE
//
// =============================================================================
void SPIWrite(int address, int data){
    
    int clr;
    
    /*Enable SPI*/
    
    /*Write address + write (0) to SPI1BUF*/
    SPI1BUFL = address;
    
    /*Wait for the transfer buffer to empty*/
    while(SPI1STATLbits.SPITBF);
        
    /*Wait for the receive buffer to fill*/
    while(SPI1STATLbits.SPIRBE);
    
    /*Read value in buffer to clear it*/
    clr = SPI1BUFL;
    
    /*Write data to SPI1BUF*/
    SPI1BUFL = data;
    
    /*Wait for the transfer buffer to empty*/
    while(SPI1STATLbits.SPITBF);
        
    /*Wait for the receive buffer to fill*/
    while(SPI1STATLbits.SPIRBE);
    
    /*Read value in buffer to clear it*/
    clr = SPI1BUFL;
    
    /*Disable SPI*/    
    
} 

// =============================================================================
//
//                                  SPI READ
//
// =============================================================================
int SPIRead(int address){
    
    int clr, data;
    
    address = address | 0x80;
    
    /*Enable SPI*/
    
    /*Write address + read (1) to SPI1BUF*/
    SPI1BUFL = address;
    
    /*Wait for the transfer buffer to empty*/
    while(SPI1STATLbits.SPITBF);
        
    /*Wait for the receive buffer to fill*/
    while(SPI1STATLbits.SPIRBE);
    
    /*Read value in buffer to clear it*/
    clr = SPI1BUFL;
    
    /*Write a dummy to SPI1BUF*/
    SPI1BUFL = 0x0000;
    
    /*Wait for the transfer buffer to empty*/
    while(SPI1STATLbits.SPITBF);
        
    /*Wait for the receive buffer to fill*/
    while(SPI1STATLbits.SPIRBE);
    
    /*Read value in buffer to clear it*/
    data = SPI1BUFL;
    
    /*Disable SPI*/
    
    return data;
    
}

// =============================================================================
//
//                                 SPI PIN INIT
//
// =============================================================================
void spiPinInit()
{
    //TODO: Define TRIS bits for sPI pins
    
    OSCCON = 0x46;
    OSCCON = 0x57;
    OSCCONbits.IOLOCK = 0;  //Unlocks PPS for writing
    
    //SET PPS
    //Inputs
    RPINR20bits.SDI1R = 1;  //Assigned SDI to pin 'RP1' (pin 5) B1
    
    //Outputs
    RPOR1bits.RP2R = 7;     //SDO1 is assigned to pin RP2 (pin 6) B2
    RPOR1bits.RP3R = 8;     //SCK1OUT is assigned to pin RP3 (pin 7) B3
    
    OSCCON = 0x46;
    OSCCON = 0x57;
    OSCCONbits.IOLOCK = 1; //Stops write;
}
