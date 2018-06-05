// Made by: JPCatarino  Date: 06/06/2018
// Este programa foi feito com o objetivo de mostrar como fazer a configuração básica de uma UART para a DETPIC32
// Contém também um pequeno exemplo de como ela pode ser usada, em que vamos transmitir o valor
// dos DIPSWITCHES para os display cada vez que recebermos o carater S
#include <detpic32.h>

// Valor Global usado mais tarde para receber valores da UART
char c;

// Usado para configurar alguns dispositivos IO que irão ser usados para o teste da UART 
void config_IO(){
    // DISPLAYS and DIP
    LATDbits.LATD5 = 1;
    TRISB = (TRISB & 0x00FF) | 0x000F;
    TRISD = TRISD & 0xFF9F;
}

void config_UART(){
    // 1º passo: configurar o gerador de baudrate de acordo com a taxa pretendida
    // BRGH (Divisão por 16 = 0 ; Divisão por 4 = 1)
    // BRG = (PBCLK/(BRGH * baudrate))-1
    U1MODEbits.BRGH = 0;
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
    IEC0bits.U1RXIE = 1;
}

// 2 funções normalmente usadas são a getc() para receber um carater e putc() para transmitir um carater

void putc(char byte2send){
    // 1º passo: Esperar que o FIFO esteja vazio
    while(U1STAbits.UTXBF == 1);
    // 2º passo: Colocar carater no registo de transmissão
    U1TXREG = byte2send;
}

char getc(void){
    // 1º passo: Dar reset a flag de erro
    if(U1STAbits.OERR == 1) U1STAbits.OERR = 0;
    // 2º passo: Esperar que o FIFO esteja disponivel para receber dados
    while(U1STAbits.URXDA == 0);
    // 3º passo: Returnar o caracter no buffer de receção 
    return U1RXREG;
}

// uma função que pode ser util é a de transmitir uma string

void putString(char *str)
{
    unsigned int i = 0;
	while(str[i] != '\0'){
        // Usar função putc() para ir pondo cada carater da string até chegar ao final
        putc(str[i]);
        i++;
    }
}

void main(void){
    config_UART();
    config_interrupts();
    EnableInterrupts();
    config_IO();
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
    while(1){
        getc();

        if(c == 'S'){
            int pval = PORTB & 0x000F;
            LATB = ((int)display7Scodes[pval]<<8);
        }
    }

}

// interrupt handler para a UART1
void _int_(24) isr_uart1(void){
    // é ativada quando recebe um char
    // esse char é atribuido então ao valor global c
    c = U1RXREG;
    IFS0bits.U1RXIF = 0;
}
