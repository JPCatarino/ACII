
void configUart(unsigned int baud, char parity, unsigned int stopbits){
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
}