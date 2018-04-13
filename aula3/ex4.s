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
	.globl main

main:	addiu 	$sp,$sp,-16 		# guardar valores 
	sw 	$ra,0 ($sp)
	sw	$s0,4 ($sp)
	sw	$s1,8 ($sp)
	sw	$s2,12($sp)
					# void main(void){
	li	$s0,0			# int v = 0;
	lui	$s1,SFR_BASE_HI		# 
	lw	$s2,TRISE($s1)		# read memory address
	andi	$s2,$s2,0xFFFE		# modify address
	sw	$t2,TRISE($s1)		# TRISE0 = 0;
					# while(1){
while:	sw	$s0,LATE($s1)		# 	LATE0 = v;
	li	$a0,20			#
	jal	delay			#	 delay(500)
	xor	$s0,$s0,1		#	 v ^= 1
	j while				# }
	
end:	lw	$ra,0 ($sp)
	lw	$s0,4 ($sp)
	lw	$s1,8 ($sp)
	lw	$s2,12($sp)
	addi	$sp,$sp,16
	jr	$ra			




#######################################################################
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
