#include <detpic32.h>

void send2displays(unsigned char value);
void delay(int ms);
unsigned char toBcd(unsigned char value);

void main(){
    LATDbits.LATD6 = 1;         // display high active 
    LATDbits.LATD5 = 0;         // display low inactive

    TRISB = TRISB & 0x00FF;     // configure RB8-RB15 as outputs
    TRISD = TRISD & 0xFF9F;     // configure RD5-RD6 as outputs

    TRISBbits.TRISB4 = 1;
    AD1PCFGbits.PCFG4 = 0;
    AD1CON1bits.SSRC = 7;

    AD1CON1bits.CLRASAM = 1;

    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = 3;

    AD1CHSbits.CH0SA = 4;
    AD1CON1bits.ON = 1;

    int i = 0;
    int V = 0;

    while(1){
        int nums[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
        delay(100);
        if(++i == 25){
            AD1CON1bits.ASAM = 1;
            while (IFS1bits.AD1IF == 0);  

            int voltage_average = 0;

            int *p = (int *)(&ADC1BUF0);
            for(; p <= (int *)(&ADC1BUFF); p+=4) { 
                // Calculate buffer average
                voltage_average += *p;
            }

            // Calculate voltage amplitude
            voltage_average /= 4;

            // Convert voltage amplitude to decimal
            V = (voltage_average*33+511)/1023;

            send2displays(toBcd(nums[V/2]));


        }
        putChar('|');
        printInt10(V/2);
        putChar('-');
        printInt10(nums[V]);
    }

}

void send2displays(unsigned char value) 
{
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 
        0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

    static char displayFlag = 0;            // variável não perde o valor entre
                                            // chamadas à função (static)
    unsigned char digit_low = value & 0x0F;
    unsigned char digit_high = value >> 4;
    // if "displayFlag" is 0 then send digit_low to display_low 
    if (!displayFlag)
    {
        LATDbits.LATD5 = 1; LATDbits.LATD6 = 0;
        LATB = (LATB & 0x00FF) | ((int)display7Scodes[digit_low] << 8);
    }
    // else send digit_high to display_high
    else 
    {
        LATDbits.LATD5 = 0; LATDbits.LATD6 = 1;
        LATB = (LATB & 0x00FF) | ((int)display7Scodes[digit_high] << 8);
        LATBbits.LATB15 = 1;
    }
    // toggle "displayFlag" variable
    displayFlag = !displayFlag;
}


void delay(int ms){
    for(; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

unsigned char toBcd(unsigned char value){
    return ((value / 10) << 4) +(value % 10);
}