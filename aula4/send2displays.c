void send2displays(unsigned char value){
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
    static char displayFlag = 0;
    unsigned char digit_high = value >> 4;
    unsigned char digit_low  = value & 0x0F;

    if(!displayFlag){
        LATDbits.LATD5 = 1; 
        LATDbits.LATD6 = 0;
        LATB = (LATB & 0x80FF) | ((int)display7Scodes[digit_low] << 8));
    }
   else{
        LATDbits.LATD5 = 0; 
        LATDbits.LATD6 = 1;
        LATB = (LATB & 0x80FF) | ((int)display7Scodes[digit_high] << 8);
   }
   displayFlag = !displayFlag; 
    
}