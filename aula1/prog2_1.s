	.equ inkey,1
	.equ printstr,8
	
	.data
	
msg:	.asciiz "Key Pressed\n"
	.text
	.globl main
	
main:	
whileone:
while:		ori $v0,$0,inkey
		syscall
		beq $v0,0,while
if:		beq $v0,'\n',endwhile
		ori $v0,$0,printstr
		la $a0,msg
		syscall
		j whileone

endwhile:	li $v0,0
		jr $ra	
