// Made by: JPCatarino  Date: 06/06/2018
// Este programa foi feito com o objetivo de mostrar como fazer a configuração básica de timers.
// Aproveita-se também para rever outros conceitos como o ADC, etc. usados para demonstrar como usar timers
// O exemplo usado foi exercicio numa das versões do 2ª teste prático de 2018

// Enunciado : Apresenta nos displays um valor entre 0-99, usando 2 amostras da adc.
//             Usa dois timers, um de 120hz, para os displays, e outro de 10hz, para conversão e amostragem.
//             Configura também um PWM de freq 120Hz e 40% de duty cycle


#include <detpic32.h>

// Int global para passar dados de função para função
volatile int value = 0;

// Configuração dos dispositivos IO utilizados (DISPLAYS)
void config_IO(){
    LATDbits.LATD6 = 1;
    LATDbits.LATD5 = 0;
    TRISB = TRISB & 0x00FF;
    TRISD = TRISD & 0xFF9F;
}

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
    
    // Neste caso vamos programar um timer com freq de 10Hz
    
    T2CONbits.TCKPS = 5;

    // 2º Passo: Determinar constante de divisão e atribuir a PRx
    // esta constante é obtida a partir de PRx = ((PBCLK / psk)/freqDesejada) - 1
    
    PR2 = 62499;

    // 3º Passo: Dar reset ao contador e ativar o timer (este é obrigatoriamente o último passo da configuração)

    TMR2 = 0;
    T2CONbits.TON = 1;

    // Seguindo os passos anteriores vamos configurar um timer de 120hz
    
    T3CONbits.TCKPS = 2;
    PR3 = 41666;
    TMR3 = 0;
    T3CONbits.TON = 1;

    // No teste pode ser pedido para configurar um módulo OC (Output Compare) para gerar um PWM
    // A seguir estão as instruções necessárias para configurar o módulo
    // O bom funcionamento pode ser verificado através de um osciloscópio.
    // Vamos configurar um OC com base no timer3 de 120hz, com um duty-cycle de 40%

    // 1º passo - Configurar o bit OCM do OCxCON com um dos seguintes valores:
    // 7 = PWM mode on OCx; Fault pin enabled
    // 6 = PWM mode on OCx; Fault pin disabled
    // 5 = Initialize OCx pin low; generate continuous output pulses on OCx pin
    // 4 = Initialize OCx pin low; generate single output pulse on OCx pin
    // 3 = Compare event toggles OCx pin
    // 2 = Initialize OCx pin high; compare event forces OCx pin low
    // 1 = Initialize OCx pin low; compare event forces OCx pin high
    // 0 = Output compare peripheral is disabled but continues to draw current

    // Neste caso como queremos gerar um PWM, vamos atribuir o valor 6
    OC1CONbits.OCM = 6;

    // 2º passo - Escolher o timer base, usando o bit OCTSEL do conjunto OCxCON com um dos valores seguintes:
    // 1 = Timer3 is the clock source for this Output Compare module
    // 0 = Timer2 is the clock source for this Output Compare module

    // Neste caso vai ser usado o timer3
    OC1CONbits.OCTSEL = 1;

    // 3º passo - Calcular a constante OCxRS que é dado por ((PRx + 1)*dutycycle)/100
    // É de notar que podemos mudar este valor sempre que necessário durante a execução do programa.
    OC1RS = 16666;

    // 4º passo - Ativar o módulo
    OC1CONbits.ON = 1;

    // DISCLAIMER: Não me foi possível testar esta configuração portanto não sei até que ponto 
    // os valores estão correctos, no entanto são estes os passos que tem de seguir ;))

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

    // Seguindo os mesmo passo para o timer3
    IPC3bits.T3IP = 2;
    IEC0bits.T3IE = 1;
    IFS0bits.T3IF = 0;

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

// Funções de suporte

void send2displays(unsigned char value)
{
    static char codes7S[]={0x3F, 0x06, 0x5b, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F,
    0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static char displayFlag = 0;
    unsigned int digit_low, digit_high;
    digit_low = value & 0x0F;
    digit_high = value >> 4;
    if (displayFlag == 0)
    {
        LATDbits.LATD6=0;
        LATB = (LATB & 0x00FF) | (codes7S[digit_low] << 8);
        LATDbits.LATD5 = 1;
    }
    else
    {
        LATDbits.LATD5=0;
        LATB = (LATB & 0x00FF) | (codes7S[digit_high] << 8);
        LATDbits.LATD6 = 1;
    }
    displayFlag = !displayFlag;
}


unsigned char toBCD(unsigned char val)
{
    return ((val / 10) << 4) + (val % 10);
}

void main(void){
    config_IO();
    config_timers();
    config_adc();
    config_interrupts();
    EnableInterrupts();

    while(1);
}

// Interrupts handlers para os timers e para adc (para os vetores, ver PIC32 Family Data Sheet pg 74-76)

void _int_(8) rsi_t2(void)
{
    // Este timer de 10Hz é usado para iniciar uma conversão da ADC 
    AD1CON1bits.ASAM = 1;
    IFS0bits.T2IF = 0;
}

void _int_(12) rsi_t3(void)
{
    // Este timer de 120 Hz é usado para mandar a informação para os displays
    send2displays(toBCD(value));
    IFS0bits.T3IF = 0;
}

void _int_(27) rsi_ADC(void)
{
    // O interrupt handler da ADC é usado para ir buscar os valores convertidos pelo módulo
    // No teste no qual este exemplo foi baseado era pedido para fazer amostragem de 2 samples
    // Por questões de simplificação aqui é usada só uma :)
    value = ((ADC1BUF0)*99)/1023;
    IFS1bits.AD1IF = 0;
}


