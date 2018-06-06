// Made by: JPCatarino  Date: 06/06/2018
// Este programa foi feito com o objetivo de mostrar como fazer a configuração básica de timers.
// Aproveita-se também para rever outros conceitos como o ADC, etc. usados para demonstrar como usar timers

#include <detpic32.h>

// Função de configuração do timer
void config_timers(void){
    // 1º Passo: Definir o prescaler e atribuir TCKPS do conjunto TxCON
    // O prescaler é calculado por psk >= PBCLK/((65535 + 1) * freqDesejada)
    // a TCKPS podem ser atribuidos os valores (em caso de timers tipo B):
    // 7 = 1:256 prescale value
    // 6 = 1:64 prescale value
    // 5 = 1:32 prescale value
    // 4 = 1:16 prescale value
    // 3 = 1:8 prescale value
    // 2 = 1:4 prescale value
    // 1 = 1:2 prescale value
    // 0 = 1:1 prescale value
    
    T2CONbits.TCKPS = 2;

    // 2º Passo: Determinar constante de divisão e atribuir a PRx
    // esta constante é obtida a partir de PRx = ((PBCLK / psk)/freqDesejada) - 1
    
    PR2 = 49999;

    // 3º Passo: Dar reset ao contador e ativar o timer (este é obrigatoriamente o último passo da configuração)

    TMR2 = 0;
    T2CONbits.TON = 1;

}

// Função de configuração de interrupts
void config_interrupts(void){
    // Basta seguir os passos genéricos de ativar interrupts, sendo necessário verificar em que grupo
    // é que se encontram os bits necessários (consultar manual do PIC32)

    // Definir prioridade
    IPC2bits.T2IP = 2;

    // Enable aos interrupts do timer
    IEC0bits.T2IE = 1;

    // Reiniciar interrupt flag
    IFS0bits.T2IF = 0;

    // Configuração dos interrupts da ADC
    IPC6bits.AD1IP = 2;
    IEC1bits.AD1IE = 1;
    IFS1bits.AD1IF = 0;
}

// função de suporte para configuração da ADC
void config_adc(void)
{
// Inicializacao da ADC
TRISBbits.TRISB4 = 1; // RBx digital output disconnected
AD1PCFGbits.PCFG4= 0; // RBx configured as analog input (AN4)
AD1CON1bits.SSRC = 7; // Conversion trigger selection bits: in this
// mode an internal counter ends sampling and
// starts conversion
AD1CON1bits.CLRASAM = 1; // Stop conversions when the 1st A/D converter
// interrupt is generated. At the same time,
// hardware clears the ASAM bit
AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns)
AD1CON2bits.SMPI = 2-1; // Interrupt is generated after XX samples
// (replace XX by the desired number of
// consecutive samples)
AD1CHSbits.CH0SA = 4; // replace x by the desired input
// analog channel (0 to 15)
AD1CON1bits.ON = 1; // Enable A/D converter
// This must the last command of the A/D
// configuration sequence
}