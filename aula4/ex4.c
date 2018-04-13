#include <detpic32.h>

void delay(int ms);

void main(){
    unsigned int segment; 
    TRISB = TRISB & 0x00FF;
    TRISD = TRISD & 0xFF9F;
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 1;
    while(1){
        LATDbits.LATD6 = !LATDbits.LATD6;
        LATDbits.LATD5 = !LATDbits.LATD5; // change displays
        segment = 1;
        int i;
        for(i = 0; i < 7; i++){
            int temp = segment << 8;
            printInt10(temp);
            putChar(' ');
            LATB = temp;
            delay(1000);
            segment = segment << 1;
        }
    }
}

void delay(int ms){
    for(; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}
