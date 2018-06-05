#include<detpic32.h>


int main(void){
    U1BRG = 10;             // 115200 bps
    U1MODEbits.BRGH = 0;    // fator divisão = 16

    U1MODEbits.PDSEL = 0;   // no parity
    U1MODEbits.STSEL = 1;   // 1 stop bit

    U1STAbits.UTXEN = 1;    // ativar UTxEN
    U1STAbits.URXEN = 1;    // ativar URxEN
    U1MODEbits.ON = 1;      // ativar UART
}