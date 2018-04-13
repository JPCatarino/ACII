	.equ SFR_BASE_HI, 0xBF88
	.equ TRISE, 0x6100
	.equ PORTE, 0x6110
	.equ TRISB, 0x6040
	.equ PORTB, 0x6050
	.equ LATE, 0x6120
	
	.data
	.text
	.globl	main
	
main:	lui 	$t1,SFR_BASE_HI # para meter RE0 como saida
	lw	$t2,TRISE($t1)
	andi	$t2,$t2,0xFFF0
	sw	$t2,TRISE($t1)
	
	lw	$t2, TRISB($t1)	# para meter RB0 como entrada
	andi	$t2,$t2,0x000F
	sw	$t2,TRISB($t1)
	
while:	lw	$t2, PORTB($t1)	# read RB0
	
	move 	$t3,$t2		# to negate bit 0 and 3 we have to move them around
	andi	$t3,$t3,0x0006
	nor	$t2,$t2,$t2
	andi	$t2,$t2,0xFFF9
	or	$t2,$t2,$t3
	
	sw	$t2, LATE($t1) 	#write RE0
	j	while
	
	jr	$ra