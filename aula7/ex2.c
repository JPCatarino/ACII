#include <detpic32.h>

void main(void){
    T3CONbits.TCKPS = 7;
    PR3 = 39062;
    TMR3 = 0;
    T3CONbits.TON = 1;

    IPC3bits.T3IP = 2;      // interrupt priority
    IEC0bits.T3IE = 1;      // Enable timer t3 int
    IFS0bits.T3IF = 0;      // Reset time T3

    EnableInterrupts();
    while(1);
}

//Interrupt Handler
void _int_(12)  isr_T3(void){
    putChar('.');
    IFS0bits.T3IF = 0;
}