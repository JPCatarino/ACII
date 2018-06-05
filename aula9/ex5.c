#include<detpic32.h>
#include "configUart.c"

void delay(int ms);
void putc(char byte2send);
void puts(char *str);

int main(void){
    configUart(115200,'N',1);

    while(1){
        puts("String de teste\n");
        delay(1000);
    }
    return 1;
}


void delay(int ms){
    for(; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
} 

void putc(char byte2send){
    while(U1STAbits.UTXBF == 1);
    U1TXREG = byte2send;
}

void puts(char *str){
    unsigned int i = 0;
    while(str[i] != '\0'){
        putc(str[i]);
        i++;
    }
}