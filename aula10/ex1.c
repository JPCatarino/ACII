#include<detpic32.h>

#define DisableUart1RxInterrupt()   IEC0bits.U1RXIE = 0
#define EnableUart1RxInterrupt()    IEC0bits.U1RXIE = 1
#define DisableUart1TxInterrupt()   IEC0bits.U1TXIE = 0
#define EnableUart1TxInterrupt()    IEC0bits.U1TXIE = 1

#define BUF_SIZE 8
#define INDEX_MASK (BUF_SIZE - 1)

typedef struct{
    unsigned char data[BUF_SIZE];
    unsigned head;
    unsigned tail;
    unsigned count;
}circularBuffer;

volatile circularBuffer txb;    //Transmission buffer
volatile circularBuffer rxb;    // Reception buffer

void comDrv_flushRx(void);
void comDrv_flushTx(void);
void comDrv_putc(char ch);
void comDrv_puts(char *s);
void comDrv_config(unsigned int baud, char parity, unsigned int stopbits);
char comDrv_getc(char *pchar);
void configInterrupts();

int main(void){
    comDrv_config(115200,'N',1);
    configInterrupts();
    comDrv_flushRx();
    comDrv_flushTx();
    EnableInterrupts();
    
    comDrv_puts("PIC32 UART Device-driver\n");
    char c;
    while(1){
        if(comDrv_getc(&c))
            comDrv_putc(c);
    }
}


void comDrv_flushRx(void){
    // Initialize variable of the reception buffer
    rxb.head = 0;
    rxb.tail = 0;
    rxb.count = 0;
}

void comDrv_flushTx(void){
    // Initialize variable of the transmission buffer
    txb.head = 0;
    txb.tail = 0;
    txb.count = 0;
}

void comDrv_putc(char ch){
    while(txb.count == BUF_SIZE){}      //wait till buffer full
    txb.data[txb.tail] = ch;            // copy character to transmission
                                        // uffer at position tail
    txb.tail = (txb.tail + 1) & INDEX_MASK; //increment tail index
    DisableUart1TxInterrupt();
    txb.count+= 1;
    EnableUart1TxInterrupt();
}

void comDrv_puts(char *s){             // send string to tran buffer
    while(*s != '\0'){
        comDrv_putc(*s);
        s++;
    }
}

void comDrv_config(unsigned int baud, char parity, unsigned int stopbits){
     U1MODEbits.BRGH = 0;    // fator divisão = 16

    // To Calculate BaudRate
    if(baud >= 600 || baud <= 115200)
        U1BRG = ((20000000 + 8 * baud)/(16*baud)) - 1;
    else    
        U1BRG = 10;

    // to parity

    if(parity == 'N')
        U1MODEbits.PDSEL = 0;
    else if(parity == 'E')
        U1MODEbits.PDSEL = 1;
    else    
        U1MODEbits.PDSEL = 2;
    
    // stop bits

    if(stopbits == 2)
        U1MODEbits.STSEL = 1;
    else
        U1MODEbits.STSEL = 0;

    // ativar UTxEN,URxEN e UART
    U1STAbits.UTXEN = 1;    
    U1STAbits.URXEN = 1;    
    U1MODEbits.ON = 1;     

    DisableUart1TxInterrupt();
	DisableUart1RxInterrupt();

/* 	//INTERRUPTS UART
	U1STAbits.URXISEL = 00;
	IPC6bits.U1IP=3;
	IFS0bits.U1RXIF=0;  */
}

void configInterrupts() {

	// UART1
	IPC6bits.U1IP = 2;		// configure priority of interrupts
	IFS0bits.U1RXIF = 0;	// clear interrupt flag (RX)
	IFS0bits.U1TXIF = 0;	// clear interrupt flag (TX)
	IEC0bits.U1RXIE = 1;	// enable interrupts (RX)
	IEC0bits.U1TXIE = 1;	// enable interrupts (TX)
}

void _int_(24) isr_uart1(void){
    if(IFS0bits.U1TXIF){
        if(txb.count > 0){
            U1TXREG = txb.data[txb.head];
            txb.head = (txb.head + 1) & INDEX_MASK;
            txb.count-=1;
        }
        if(txb.count == 0){
            DisableUart1TxInterrupt();
        }
        IFS0bits.U1TXIF = 0;
    }
    if(IFS0bits.U1RXIF){
        rxb.data[rxb.tail] = U1RXREG; 
		rxb.tail = (rxb.tail + 1) & INDEX_MASK;
		if (rxb.count < BUF_SIZE) 
		{
			rxb.count++;
		}
		else {
			rxb.head = (rxb.head + 1) & INDEX_MASK;
		}
    	IFS0bits.U1RXIF = 0;
    }
}


char comDrv_getc(char *pchar){
    if(rxb.count == 0){
        return 0;
    }
    DisableUart1RxInterrupt();
    *pchar = rxb.data[rxb.head];
    rxb.count-=1;
    rxb.head = (rxb.head + 1) & INDEX_MASK;
    EnableUart1RxInterrupt();

    if(*pchar == 'S')
        comDrv_puts("por favor ajudem, eu estou preso num loop infinito");
    return 1;
}