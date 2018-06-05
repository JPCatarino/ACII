#include<detpic32.h>

char c;

void config_Uart(void){
    U1MODEbits.BRGH = 0;
    U1BRG = 10;
    U1MODEbits.PDSEL = 0;
    U1MODEbits.STSEL = 1;

    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    U1MODEbits.ON = 1;
}

void config_Interrupts(void){
    IPC6bits.U1IP = 2;		// configure priority of interrupts
	IFS0bits.U1RXIF = 0;	// clear interrupt flag 
	IEC0bits.U1RXIE = 1;	// enable interrupts
}

void config_IO(void){
    TRISB = TRISB & 0x000F;
}

void putc(char byte2send){
    while(U1STAbits.UTXBF == 1);
    U1TXREG = byte2send;
}

char getc(void){
    while(U1STAbits.URXDA == 0);
    return U1RXREG;
}

void putString(char *str)
{
    unsigned int i = 0;
	while(str[i] != '\0'){
        putc(str[i]);
        i++;
    }
}

void main(void){
    config_IO();
    config_Uart();
    config_Interrupts();
    EnableInterrupts();
    while(1){
        getc();

        if(c == 'L'){
            int val = PORTB & 0x000F;
            char send = val + '0';
            putc(send);
            putc('\n');
        }
        if(c == 'S'){
            putString("Jorge Catarino\n");
        }
        if(c == 'Q'){
            exit(0);
        }
    }
}

void _int_(24) isr_uart1(void){
    c = U1RXREG;
    IFS0bits.U1RXIF = 0;
}

