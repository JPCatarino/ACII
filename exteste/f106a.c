#include <detpic32.h>


void main(void){
    TRISE = TRISE & 0xFFF0;
    TRISB = TRISB & 0x000F;

    while(1){
        LATEbits.LATE3 = PORTBbits.RB0;
        LATEbits.LATE2 = PORTBbits.RB1;
        LATEbits.LATE1 = PORTBbits.RB2;    
        LATEbits.LATE0 = PORTBbits.RB3;
        }
}