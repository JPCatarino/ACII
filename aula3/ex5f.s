	.equ SFR_BASE_HI, 0xBF88 		# 16 MSbits of SFR area
	.equ TRISE, 0x6100 				# TRISE address is 0xBF886100
	.equ PORTE, 0x6110 				# PORTE address is 0xBF886110
	.equ LATE,  0x6120 				# LATE  address is 0xBF886120

	.equ TRISB, 0x6040 				# TRISB address is 0xBF886040
	.equ PORTB, 0x6050 				# PORTB address is 0xBF886050
	.equ LATB,  0x6060 				# LATB  address is 0xBF886060

	.equ PUT_CHAR, 3
	.equ PRINT_INT, 6
	.equ READ_CORE_TIMER, 11 
	.equ RESET_CORE_TIMER, 12

	.data

	.text
	.globl main
		
main:   addu 	$sp, $sp, -4
	sw 	$ra, 0($sp)			# salvaguardar retorno

	lui 	$t1, SFR_BASE_HI 		# Initializar base address
	lw 	$t2, TRISE($t1) 		# 
	andi 	$t2, $t2, 0xFFF0 		# 
	sw 	$t2, TRISE($t1)			# RE[3-0] - outputs
	lw 	$t2, TRISB($t1) 		# 
	ori 	$t2, $t2, 0x0004 		# 
	sw 	$t2, TRISB($t1)			# RB[2] - input

	li 	$s0, 0				# init
	
while:	lw 	$t0, PORTB($t1)			# while(1){
	andi 	$t0, $t0, 0x0004		# 	ler entradas
	beq	$t0,$0,else			# 	if( RB[2] == 1){
	andi	$t2,$s0,0x0008			# 		verificar bit mais significatibo
	bne	$t2,$0,elses			#		if(msb == 0){
	sll	$s0,$s0,1			#			counter << 1
	ori 	$s0, $s0, 0x0001		#			lsb = 1
	j 	endif				# 		}

elses:	sll	$s0,$s0,1			# 	else{ counter << 1
	andi	$s0,$s0,0xFFFE			#	      lsb = 0
	j	endif				#
	
else:	andi	$t2,$s0,0x0001			#	verificar bit menos significativo
	bne	$t2,$0,elsed			#	if(lsb == 0){
	srl	$s0,$s0,1			#		counter >> 1
	ori	$s0,$s0,0x0008			#		msb = 1
	j	endif				#

elsed:	srl	$s0,$s0,1			#	else{ counter >> 1
	andi	$s0,$s0,0xFFF7			#	      msb = 0 }  	

endif: 	andi 	$s0,$s0,0x000F			# contar só os bits menos significativos do registo
       	lw 	$t2,LATE($t1) 			#   Ler o registo LATE
       	andi 	$t2,$t2,0xFFF0 			#   Modificar os bits menso sig
       	or 	$t2,$t2,$s0			#   
       	sw 	$t2,LATE($t1)			#   escrever resultado nos bits RE[3..0]

       	li 	$a0, 667			#
       	jal 	delay				#
       
       	j	while				# }
       
       	lw 	$ra, 0($sp)			# retorna ra
       	addu 	$sp, $sp, 4			#
       	jr 	$ra				# EOF

#########################################################
#delay()

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