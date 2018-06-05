#include<detpic32.h>

volative int temp = 0;
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
    
    //interrupts
    IPC6bits.AD1IP = 2;
    IEC1bits.AD1IE = 1;
    IFS1bits.AD1IF = 0;
}

void config_timers(void)
{
    T2CONbits.TCKPS = 1;
    PR2 = 49999;
    TMR2 = 0;
    T2CONbits.TON = 1;

    T3CONbits.TCKPS = 3;
    PR3 = 49999;
    TMR3 = 0;
    T3CONbits.TON = 1;

    //Configure timer interrupts

    IPC2bits.T2IP = 2;
    IEC0bits.T2IP = 1;
    IFS0bits.T2IF = 0;

    IPC3bits.T3IP = 2;
    IEC0bits.T3IP = 1;
    IFS0bits.T3IF = 0;

}

void configOC(void)
{
    OC2CONbits.OCM = 6;
    OC2CONbits.OCTSEL = 0;
    OC2RS = 12500;
    OC1CONbits.ON = 1; 
}

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

void main(void)
{
    config_adc();
    config_timers();
    configOC();
    LATDbits.LATD6 = 1;
    LATDbits.LATD5 = 0;

    TRISB = TRISB & 0x00FF;
    TRISD = TRISD & 0xFF9F;

    EnableInterrupts();
}

void _int_(8) isr_T2(void)
{
    send2displays(toBCD(voltage));
    IFS0.T2IF = 0;
}

void _int_(12) isr_t3(void)
{
    AD1CON1bits.ASAM = 1;
    IFS0bits.T3IF = 0;
}

void _int_(27) isr_adc(void)
{
    voltage = ((ADC1BUF0)*70)/1023;
    IFS1bits.AD1IF = 0;
}