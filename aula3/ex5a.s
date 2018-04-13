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
	
main:	addiu 	$sp,$sp,-20 		# guardar valores 
	sw 	$ra,0 ($sp)
	sw	$s0,4 ($sp)
	sw	$s1,8 ($sp)
	sw	$s2,12($sp)
	sw	$s3,16($sp)
	
	
	lui 	$s1,SFR_BASE_HI # para meter RE0 como saida
	lw	$s2,TRISE($s1)
	andi	$s2,$s2,0xFFF0
	sw	$s2,TRISE($s1)
	
	lw	$s2, TRISB($s1)	# para meter RB0 como entrada
	andi	$s2,$s2,0x000F
	sw	$s2,TRISB($s1)
	
init:	li	$s3,0

while:	sw	$s3,LATE($s1)
	li	$a0,500			#
	jal	delay			#	 delay(500)
	beq	$s3,15,init
	addi	$s3,$s3,1		
	j while
	
end:	lw	$ra,0 ($sp)
	lw	$s0,4 ($sp)
	lw	$s1,8 ($sp)
	lw	$s2,12($sp)
	addi	$sp,$sp,16
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
