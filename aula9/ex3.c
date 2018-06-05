#include<detpic32.h>

void delay(int ms);
void putc(char byte2send);
void puts(char *str);

int main(void){
    U1BRG = 10;             // 115200 bps
    U1MODEbits.BRGH = 0;    // fator divisão = 16

    U1MODEbits.PDSEL = 0;   // no parity
    U1MODEbits.STSEL = 1;   // 1 stop bit

    U1STAbits.UTXEN = 1;    // ativar UTxEN
    U1STAbits.URXEN = 1;    // ativar URxEN
    U1MODEbits.ON = 1;      // ativar UART

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