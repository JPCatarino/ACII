#include <detpic32.h>

void delay(int ms);
void send2displays(unsigned char value);

volatile unsigned char voltage = 0;

void main(){
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
    IFS1bits.AD1IF = 0;                     // clear A/D interrupts flag

    EnableInterrupts();
    int i = 0;
    while(1){
        delay(10);
        if(i++ == 25){
            AD1CON1bits.ASAM = 1;
            i = 0;
        }
    send2displays(voltage);
    }
}

void delay(int ms){
    for(; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
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

void _int_(27) isr_Adc(void){
    int volt = 0;
    int *p = (int *)(&ADC1BUF0);
    int i;
    for(i = 0;i < 8;i++){
        printInt(p[i*4], 10);
        volt += p[i*4];
        putChar(' ');
    }
    putChar('\n');

    volt /= 8;
    int temp = (volt*33+511)/1023;
    int inteiro = temp / 10;
    int decimal = temp % 10;

    voltage = (inteiro & 0x000F) << 4 | (decimal & 0x000F);

    IFS1bits.AD1IF = 0;
}
