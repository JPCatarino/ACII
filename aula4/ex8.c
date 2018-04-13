#include<detpic32.h>

void main(){
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
    TRISB = (TRISB & 0x80FF) | 0x000F;  // RB[3..0] = input RB[14..8] = output
    TRISD = TRISD & 0xFF9F;            
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 1;
    
    while(1){
       int pval = PORTB & 0x000F;
       LATB = ((int)display7Scodes[pval]<<8);
       }
}
