#include <detpic32.h>
#include "spi2EEPROM.h"


int main(void){
    char status;
    spi2_init();
    spi2_setClock(EEPROM_CLOCK);
    for(;;)
    {
        status = eeprom_readStatus();
        printInt(status, 2 | 4 << 16);
    }
}


