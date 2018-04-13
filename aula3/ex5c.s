	.equ SFR_BASE_HI, 0xBF88

	.equ TRISE, 0x6100
	.equ PORTE, 0x6110
	.equ TRISB, 0x6040

	.equ PORTB, 0x6050
	.equ LATE, 0x6120
	.equ RESET_CORE_TIMER,12
	.equ READ_CORE_TIMER,11

	.equ PUT_CHAR, 3
	.equ PRINT_INT, 6
		
	.data
	.text
	.globl	main
	
main:	addiu 	$sp,$sp,-4 		# guardar valores 
	sw 	$ra,0 ($sp)
	
	
	lui 	$s1,SFR_BASE_HI # para meter RE0 como saida
	lw	$s2,TRISE($s1)
	andi	$s2,$s2,0xFFF0
	sw	$s2,TRISE($s1)
	
	lw	$s2, TRISB($s1)	# para meter RB0 e RB3 como entrada
	andi	$s2,$s2,0x0008
	sw	$s2,TRISB($s1)
	
init_b:	li	$s3,0
	j 	while
init_d:	li	$s3,16
	j	while	

while:	lw	$t0,PORTB($s1)
	andi	$t0,$t0,0x0008
	beq	$t0,$0,else	# if(rb3 == 1){
	addi	$s3,$s3,1	# 	s++;	
	j	endif		#
				# }
else:	addi	$s3,$s3,-1	# else{s--;}
	j	endif		#

endif:	
	sw	$s3,LATE($s1)
	li 	$a0,500
	
	jal	delay
	bgt	$s3,15,init_b
	li	$a0,' '
	li	$v0,PUT_CHAR
	syscall
	move	$a0,$s3
	li	$a1,10
	li	$v0,PRINT_INT
	syscall
	
	beq	$s3,0,init_d
	j	while	

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