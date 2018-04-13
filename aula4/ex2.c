#include <detpic32.h>

void delay(int ms);

void main(){
    TRISE = TRISE & 0xFFF0; // set TRISE[3..0] as an output
    int cont = 0;
    while(1){
        LATE = cont;
        delay(250);
        if(cont > 15){
            cont = 0;
        }
        cont++;
        LATE = 0;
    }

}

void delay(int ms){
    for(; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}