	.equ getChar,2
	.equ putChar,3
	
	.data
	.text
	.globl main

main:
whileone:
while:		ori $v0,$0,getChar
		syscall
if:		beq $v0,'\n',endwhile
		move $a0,$v0
		ori $v0,$0,putChar
		syscall
		j whileone

endwhile:	li $v0,1
		jr $ra
