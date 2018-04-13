	.equ SFR_BASE_HI, 0xBF88
	.equ TRISE, 0x6100
	.equ PORTE, 0x6110
	.equ TRISB, 0x6040
	.equ PORTB, 0x6050
	.equ LATE, 0x6120
	.equ RESET_CORE_TIMER,12
	.equ READ_CORE_TIMER,11
	
	.data
	.text
	.globl	main
	
main:	addiu 	$sp,$sp,-4		# guardar valores 
	sw 	$ra,0 ($sp)
	
	
	lui 	$s1,SFR_BASE_HI # para meter RE0 como saida
	lw	$s2,TRISE($s1)
	andi	$s2,$s2,0xFFF0
	sw	$s2,TRISE($s1)
	
	lw	$s2, TRISB($s1)	# para meter RB0 como entrada
	andi	$s2,$s2,0x000F
	sw	$s2,TRISB($s1)
	
init:	li	$s3,15

while:	sw	$s3,LATE($s1)
	li	$a0,250			#
	jal	delay			#	 delay(250)
	beq	$s3,0,init
	addi	$s3,$s3,-1		
	j while
	
end:	lw	$ra,0 ($sp)
	addi	$sp,$sp,4
	jr	$ra		
	
###################################################	

delay:	

for:	ble	$a0,0,endfor
	li	$v0,RESET_CORE_TIMER
	syscall

whild:	li	$v0,READ_CORE_TIMER
	syscall
	blt	$v0,20000,whild
	addi	$a0,$a0,-1
	j for

endfor: jr	$ra
