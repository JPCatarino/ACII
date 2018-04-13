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
	andi	$t2,$t2,0xFFFE
	sw	$t2,TRISE($t1)
	
	lw	$t2, TRISB($t1)	# para meter RB0 como entrada
	andi	$t2,$t2,0x0001
	sw	$t2,TRISB($t1)
	
while:	lw	$t2, PORTB($t1)	# read RB0
	not	$t2,$t2
	sw	$t2, LATE($t1) #write RE0
	j	while
	
	jr	$ra
	