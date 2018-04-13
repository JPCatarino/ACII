#void main(void)
#{
#  int counter = 0;
#  while (1)
#  {
#    while (readCoreTimer() < 200000);
#    resetCoreTimer();
#    printInt(counter++, 10);
#    putchar(' ');
#  }
#}

	.equ 	READ_CORE_TIMER,11
	.equ 	RESET_CORE_TIMER,12
	.equ	PUT_CHAR,3
	.equ	PRINT_INT,7

	.data
	.text
	.globl main

main: 	li      $t0,0

while:	li      $v0,READ_CORE_TIMER
	syscall
	blt     $v0,20000000,while
	li      $v0,RESET_CORE_TIMER
	syscall
	li	$a0,' '
	li	$v0,PUT_CHAR
	syscall
	addi	$t0,$t0,1
	jal delay
	move	$a0,$t0
	li	$a1,10
	li	$v0,PRINT_INT
	syscall
	j 	while
	jr	$ra
##############################################################
delay:	

for:	ble	$a0,0,endfor
	li	$v0,RESET_CORE_TIMER
	syscall

whild:	li	$v0,READ_CORE_TIMER
	syscall
	blt	$v0,200000,whild
	addi	$a0,$a0,-1
	j for

endfor: jr	$ra