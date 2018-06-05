#include <detpic32.h>

void config_UART(){
    // 1º passo: configurar o gerador de baudrate de acordo com a taxa pretendida
    // BRGH (Divisão por 16 = 0 ; Divisão por 4 = 1)
    // BRG = (PBCLK/(BRGH * baudrate))-1
    U1BRGbits.BRGH = 0;
    U1BRG = 10;
    // 2º passo: declarar parametros da trama: 
    // dimensão da palavra a transmitir e tipo de paridade (PDSEL)
    // número de stopbits (STSEL)
    // PDSEL
    // 11 = 9-bit data, no parity
    // 10 = 8-bit data, odd parity
    // 01 = 8-bit data, even parity
    // 00 = 8-bit data, no parity
    // STSEL
    // 1 = 2 Stop bits
    // 0 = 1 Stop bit
    U1MODEbits.PDSEL = 0;
    U1MODEbits.STSEL = 0;
    // 3º passo
    // ativar os módulos de transmissão e receção (UTXEN,URXEN)
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    // 4ª passo
    // ativar a UART!
    U1MODEbits.ON = 1;
}

void config_interrupts(){
    // 1º passo: Configurar a prioridade (comum a todos os interrupts)
    IPC6bits.U1IP = 2;
    // 2º passo: É necessário dar reset a flag e dar enable aos interrupts
    // temos 2 tipos de interrupts configuraveis: Transmissão (UxTXxx) ou Receção (UxRXxx)
    IFS0bits.U1RXIF = 0;
    IES0bits.U1RXIE = 1;
}