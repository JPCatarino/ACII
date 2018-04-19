#include <detpic32.h>

void configureAll();
void send2displays(unsigned char value);

volatile unsigned char voltage = 0;

void main(void){
    configureAll();
    IFS0bits.T1IF = 0;          // clear t1 flag
    IFS0bits.T3IF = 0;          // clear t3 flag
    IFS1bits.AD1IF = 0;         // clear adc flag            

    EnableInterrupts();
    while(1);
}

void configureAll(){
    LATDbits.LATD6 = 1;                     // activate display high and deactivate low
    LATDbits.LATD5 = 0;

    TRISB = TRISB & 0x00FF;
    TRISD = TRISD & 0xFF9F;

    TRISBbits.TRISB4 = 1;                   // disconnect digital output RB4
    AD1PCFGbits.PCFG4 = 0;                  // Configure RB4 as analog
    AD1CON1bits.SSRC = 7;                   // conversion trigger selection bits
                                            //
    AD1CON1bits.CLRASAM = 1;                // stop conversion when 1st a/d converter interrupt is generated
    AD1CON3bits.SAMC = 16;                  // sample time is 16 TAD
                                            //
    AD1CON2bits.SMPI = 0;                   // conversion will be done once (x+1)
    AD1CHSbits.CH0SA = 4;                   // Select AN4 as input for A/D
                                            //
    AD1CON1bits.ON = 1;                     //
                                            //
    IPC6bits.AD1IP = 2;                     // configuring priority of A/D interrupts
    IEC1bits.AD1IE = 1;                     // enable A/D interrups
 

    T1CONbits.TCKPS = 7;
    PR1 = 19530;
    TMR1 = 0;
    T1CONbits.TON = 1;

    IPC1bits.T1IP = 2;
    IEC0bits.T1IE = 1;
    

    T3CONbits.TCKPS = 5;
    PR3 = 62499;
    TMR3 = 0;
    T3CONbits.TON = 1;

    IPC3bits.T3IP = 2;      
    IEC0bits.T3IE = 1;      
    

}

void send2displays(unsigned char value){
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
    static char displayFlag = 0;
    unsigned char digit_high = value >> 4;
    unsigned char digit_low  = value & 0x0F;

    if(!displayFlag){
        LATDbits.LATD5 = 1; 
        LATDbits.LATD6 = 0;
        LATB = (LATB & 0x80FF) | ((int)display7Scodes[digit_low] << 8);
    }
   else{
        LATDbits.LATD5 = 0; 
        LATDbits.LATD6 = 1;
        LATB = (LATB & 0x80FF) | ((int)display7Scodes[digit_high] << 8);
   }
   displayFlag = !displayFlag; 
}


void _int_(4) isr_T1(void){
    printInt10(1);
    AD1CON1bits.ASAM = 1;
    IFS0bits.T1IF = 0;
}

void _int_(12) isr_T3(void){
    printInt10(3);
    send2displays(voltage);
    IFS0bits.T3IF = 0;
}

void _int_(27) isr_Adc(void){
    int volt = 0;
    int *p = (int *)(&ADC1BUF0);
    int i;
    for(i = 0;i < 8;i++){
        volt += p[i*4];
    }
    volt /= 8;
    int temp = (volt*33+511)/1023;
    int inteiro = temp / 10;
    int decimal = temp % 10;

    voltage = (inteiro & 0x000F) << 4 | (decimal & 0x000F);

    IFS1bits.AD1IF = 0;
}