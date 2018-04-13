#include<detpic32.h>




int main (void){
	LATB = LATB & 0x0011;	// Todos os portos dos LEDS a '0'	
	TRISB = TRISB & 0xFF00;	//  a RB15 a RB8 configurados como saídas 				 
	
	TRISBbits.TRISB4 = 1; 	// RBx digital output disconnected
	AD1PCFGbits.PCFG4= 0; 	// RBx configured as analog input (AN4)
	AD1CON1bits.SSRC = 7; 	// Conversion trigger selection bits: in this
				// mode an internal counter ends sampling and
				// starts conversion
	AD1CON1bits.CLRASAM = 1;// Stop conversions when the 1st A/D converter
				// interrupt is generated. At the same time,
				// hardware clears the ASAM bit
	AD1CON3bits.SAMC = 16; 	// Sample time is 16 TAD (TAD = 100 ns)
	AD1CON2bits.SMPI = 16-1;// Interrupt is generated after XX samples
				// (replace XX by the desired number of
				// consecutive samples)
	AD1CHSbits.CH0SA = 4; 	// replace x by the desired input
				// analog channel (0 to 15)
	AD1CON1bits.ON = 1; 	// Enable A/D converter
				// This must the last command of the A/D
			 	// configuration sequence 

	int i = 0; 
	while(1){
	int velocidade [] = {250,500,750,1000,1250,1500,1750,2000,2000};
		AD1CON1bits.ASAM = 1; // Start conversion
		while( IFS1bits.AD1IF == 0 ); // Wait while conversion not done 
		int *p = (int *)(&ADC1BUF0);
		int soma = 0;
		int media = 0; 
		for( i = 0; i < 16; i++ ){ 	
			if(i<= AD1CON2bits.SMPI){
				soma += (p[i*4]*33)/1023;
			}		
		} 	
		media = soma/(AD1CON2bits.SMPI+1);
		printInt(velocidade[media/4], 2<<16|10);
		putChar('\t');
		printInt(media, 2<<16|10);
		putChar('\r');

	}
}


