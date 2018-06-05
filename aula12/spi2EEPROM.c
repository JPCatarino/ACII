#include <detpic32.h>
#include "spi2EEPROM.h"

void spi2_setClock(unsigned int clock_freq)
{
    SPI2BRG = (PBCLK + clock_freq ) / (2 * clock_freq ) - 1;
}

void spi2_init(void)
{
    volatile char trash;
    SPI2CONbits.ON = 0;         // disable SPI2 MODULE
    
    SPI2CONbits.CKP = 0;        // configure clock idle statet as logic level0
    SPI2CONbits.CKE = 1;        // configure  the clock active transition from ative to idle
    SPI2CONbits.SMP = 0;        // configure SPI data imput sample phase bit
    
    SPI2CONbits.MODE16 = 0;     // set word length

    SPI2CONbits.ENHBUF = 1;     // enable enhanced buffer

    SPI2CONbits.MSSEN = 1;      // enable master mode slave select
    SPI2CONbits.MSTEN = 1;      // enable master

    // cleaning RX FIFO

    while(SPI2STATbits.SPIRBE == 0){
        trash = SPI2BUF;
    }

    SPI2STATbits.SPIROV = 0;    // reset overflow flag     
    
    SPI2CONbits.ON = 1;         // enable SPI2
}

char eeprom_readStatus(void)
{
    volatile char trash;

    // Clear RX FIFO

    while(SPI2STATbits.SPIRBE == 0){
        trash = SPI2BUF;
    }

    SPI2STATbits.SPIROV = 0;        // reset overflow flag

    SPI2BUF = RDSR;                 // send RDSR command
    SPI2BUF = 0;                    // SEND anythign so that EEProm clocks data into SO

    while(SPI2STATbits.SPIBUSY);    // wait while SPI module is working    

    trash = SPI2BUF;                

    return SPI2BUF;

}