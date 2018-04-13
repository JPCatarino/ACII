#include <detpic32.h>

void main(){
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
    while(1){                               //
        AD1CON1bits.ASAM = 1;               //start conversion
        while(IFS1bits.AD1IF == 0);         // wait while conversion not done
        printInt(ADC1BUF0, 16 | 3 << 16);   // print converted value
        IFS1bits.AD1IF = 0;                 // reset port
    }

}