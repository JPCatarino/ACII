#include <detpic32.h>

int main(void){
    TRISD = TRISD & 0xFFFd;

	while(1){
		LATDbits.LATD1 = 1;
	}
	return 0;
}

