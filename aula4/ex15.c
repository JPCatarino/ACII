#include <detpic32.h>

void send2displays(unsigned char value);
void delay(int ms);
void onAndOff();
unsigned char toBcd(unsigned char value);

int main(){
    unsigned char counter = 0;
    int i;
    LATDbits.LATD6 = 1; 	 
    LATDbits.LATD5 = 0;
    TRISB = TRISB & 0x00FF;
    TRISD = TRISD & 0xFF9F;
    while(1){
        i = 0;
        do{
            if(counter == 0){
                onAndOff(counter++);
            }
            delay(10);
            send2displays(toBcd(counter));
        }while(++i < 100);
        
	    counter = (counter+1) % 60; // incrementa em módulo 60
    }
    return 0;
}

void send2displays(unsigned char value){
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
    static char displayFlag = 0;
    unsigned char digit_high = value >> 4;
    unsigned char digit_low  = value & 0x0F;

    if(!displayFlag){
        LATDbits.LATD5 = 1; 
        LATDbits.LATD6 = 0;
        LATB = (LATB & 0x00FF) | ((int)display7Scodes[digit_low] << 8);
        if(value % 2 == 0){             // se o numero for par, o ponto acende
            LATBbits.LATB15 = 1;
        }
    }
   else{
        LATDbits.LATD5 = 0; 
        LATDbits.LATD6 = 1;
        LATB = (LATB & 0x00FF) | ((int)display7Scodes[digit_high] << 8);
        if(value % 2 != 0){ // se o numero for impar o ponto acende
            LATBbits.LATB15 = 1;
        }
   }
   displayFlag = !displayFlag; 
    
}
void delay(int ms){
    for(; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

unsigned char toBcd(unsigned char value){
    return ((value / 10) << 4) +(value % 10);

}

void onAndOff(){
    int i = 0;
    do{
        LATB = (LATB & 0x00FF) | 0x3F00;
        LATDbits.LATD5 = 1; 
        LATDbits.LATD6 = 1;
        printInt10(i);
        delay(500);
        LATB = (LATB & 0x00FF);
        delay(500);
    }while(++i < 5);
}