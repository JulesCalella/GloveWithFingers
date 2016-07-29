#include <xc.h>
#include "config.h"

// =============================================================================
//
//                              OSCILLATOR INITIALIZE
//
// =============================================================================
void oscInit()
{
    TRISB = 0x00;
    LATB = 0x00;
    
    CLKDIVbits.RCDIV = 0; // 4MHz
    
    // Read COSC bits 
        
    // Unlock sequence high Byte of OSCCON 78h and 9Ah
    __builtin_write_OSCCONH(0x78);
    __builtin_write_OSCCONH(0x9A);
    
    // Write values to NOSC
    __builtin_write_OSCCONH(0x01);
    
    // Unlock sequence low Byte OSCCON 46h and 57h
    __builtin_write_OSCCONL(0x46);
    __builtin_write_OSCCONL(0x57);
    
    // Set OSWEN bits to switch
    __builtin_write_OSCCONL(0x01);
    
    // LOCK and CF automatically cleared
    // Hardware waits till PLL lock is detected
    // OSWEN bit is cleared automatically
    while(OSCCONbits.OSWEN != 0) continue;       
}

// =============================================================================
//
//                              ADC INITIALIZE
//
// =============================================================================
void adcInit()
{
    //p. 356
    
    AD1CON1bits.ADON = 0;       // Start with module off if it's on
    
    // 1) Configure port pins as analog
    //  AN7 RB13 Pin24
    TRISBbits.TRISB13 = 1;      //input 
    ANSBbits.ANSB13 = 1;         //analog
    
    // 2) Select voltage reference source (AD1CON2<15:13>)
    AD1CON2bits.PVCFG = 0x0;    //AVdd top reference
    AD1CON2bits.NVCFG0 = 0x0;    //AVss bottom reference
    
    // 3) Select positive and negative multiplexer (AD1CHS<15:0>)
    AD1CHSbits.CH0NB = 0x0;
    AD1CHSbits.CH0SB = 0x7;     //AN7
    AD1CHSbits.CH0NA = 0x0;
    AD1CHSbits.CH0SA = 0x7;
    
    // 4) Conversion clock (AD1CON3<7:0>) minimum TAD of 75 ns
    AD1CON3bits.ADRC = 0;       //system clock
    AD1CON3bits.ADCS = 0x01;
    
    // 5) Sample/Conversion sequence (AD1CON1<7:4> & AD1CON3<12:8>)
    AD1CON1bits.SSRC = 0x0;
    AD1CON1bits.ASAM = 0;       //begins when SAMP is manually set
    
    // 6) Channel A scanning operations (AD1CSSH & AD1CSSL)
    AD1CSSL = 0x0080;
    AD1CSSH = 0x0000;
    
    // 7) AD1CON1<9:8> & AD1CON5
    AD1CON5 = 0x0000;
    AD1CON1bits.FORM = 0x1;
    
    // 8) Interrupt rate (AD1CON2<6:2>)
    
    // 9) Turn on AD module (AD1CON1<15>)
    AD1CON1bits.ADON = 1;
    // INTERRUPTS
    //  - Clear IF (IFS0<13>)
    //  - Set enable (IEC0<13>)
    //  - Set priority (IPC3<6:4>)
    
    
}