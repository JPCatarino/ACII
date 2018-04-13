# *************************************************************************
# Programa 2 – teste do system call "inkey()"
# *************************************************************************
		.equ	INKEY,1
		.equ	PRINT_STR,8


		.data
msg:	.asciiz "Key Pressed\n"


		.text
		.globl main

main:
whileout:
while:	ori $v0,$0,INKEY
		syscall
		beq $v0,0,while
if:		beq $v0,'\n',endwhile
		ori $v0,$0,PRINT_STR
		la $a0,msg
		syscall
		j whileout
endwhile:
		li $v0,0
		jr 	$ra
