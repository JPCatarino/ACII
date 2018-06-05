#include <detpic32.h>

#define I2C_READ    1
#define I2C_WRITE   0
#define I2C_ACK     0
#define I2C_NACK    1

#define SENS_ADDRESS    0x4D
#define ADDR_WR         ((SENS_ADDRESS << 1) | I2C_WRITE) 
#define ADDR_RD         ((SENS_ADDRESS << 1) | I2C_READ)
#define TC74_CLK_FREQ   100000
#define RTR             0

void i2c1_init(unsigned int clock_freq){
    I2C1BRG = PBCLK + clock_freq / (2 * clock_freq) - 1;    //configure baudrate
    I2C1CONbits.ON = 1;                                     // Enable I2C
}

void i2c1_start(void){
    while((I2C1CON & 0x1F) != 0);                           // wait till lower 5 bits of I2C are equal 0
    I2C1CONbits.SEN = 1;                                    // activate start event
    while(I2C1CONbits.SEN != 0);                            // wait for module to complete start event
}

void i2c1_stop(void){
    while((I2C1CON & 0x1F) != 0);                           // wait will lower 5 bits of I2C are equal 0
    I2C1CONbits.PEN = 1;                                    // activate stop event
    while(I2C1CONbits.PEN != 0);                            // wait for module to complete stop event
}

int i2c1_send(unsigned char value){
    I2C1TRN = value;                                        // copy value to transmissio nregister
    while(I2C1STATbits.TRSTAT == 1);                        // wait while transmission is done
    return I2C1STATbits.ACKSTAT;                            // return ack state
}

char i2c1_receive(char ack_bit){
    while((I2C1CON & 0x1F) != 0);                           // wait till lower 5 bits of I2C are equal 0
    I2C1CONbits.RCEN = 1;                                   // Activate enable bit from receive register
    while(I2C1STATbits.RBF != 1);                           // wait while the byte is not received

    I2C1CONbits.ACKDT = (ack_bit == 0 || ack_bit == 1) ? ack_bit : 0; //copy ack_bit to ACKDT
    while((I2C1CON & 0x1F) != 0);                                 //wait will lower 5 bits are 0
    I2C1CONbits.ACKEN = 1;                                        // activate acknowledge sequence

    while(I2C1CONbits.ACKEN == 1);                                // wait for sequence to be completes

    return I2C1RCV;                                               // return received value
}

void delay(int ms){
    for(; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
} 

int main(void){
    int ack,temperature;
    i2c1_init(TC74_CLK_FREQ);
    while(1){
        i2c1_start();                                             // send start event
        ack  = i2c1_send(ADDR_WR);                                // send address + WR; copy to ack
        ack += i2c1_send(RTR);                                    // send command (RTR); add to ack
        i2c1_start();                                             // send start again
        ack += i2c1_send(ADDR_RD);                                // send adress + RD; add to ack

        if(ack != 0){                                             // test if ack to see if its != 0, if yes send error
            i2c1_stop();
            printStr("ERROR: Something isn't adding up (or is it?)");
        }
        temperature = i2c1_receive(I2C_NACK);
        i2c1_stop();

        printStr("Temperature = ");
		printInt10(temperature);
        printStr("\n");

        delay(250);
    }
}
