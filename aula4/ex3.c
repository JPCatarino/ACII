#include <detpic32.h>


void main(){
    TRISB = TRISB & 0x00FF;
    TRISD = TRISD & 0xFF9F;
    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 0;
    while(1){
        switch(getChar()){
            case 'a' :
                LATBbits.LATB8 = 1;
                break;
            case 'b':
                LATBbits.LATB9 = 1;
                break;
            case 'c':
                LATBbits.LATB10 = 1;
                break;
            case 'd':
                LATBbits.LATB11 = 1;
                break;
            case 'e':
                LATBbits.LATB12 = 1;
                break;
            case 'f':
                LATBbits.LATB13 = 1;
                break;
            case 'g':
                LATBbits.LATB14 = 1;
                break;
            case '.':
                LATBbits.LATB15 = 1;
                break;
            default:
                LATB = 0;
        }
    }
}