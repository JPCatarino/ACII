	.equ	STR_MAX_SIZE,20
	.equ	readStr,9
	.equ	printInt,6
	.equ	printStr,8
	.equ	printInt10,7
	.data
	
str1:	.space 	STR_MAX_SIZE+1
str2:	.space  STR_MAX_SIZE+1
str3:	.space  STR_MAX_SIZE+STR_MAX_SIZE +1
prt:	.asciiz "Introduza 2 strings: "
prt2:	.asciiz "Resultados:\n"
prt3:   .asciiz "\n"
	.text
	.globl  main

main:	li $v0,printStr
	la $a0,prt
	syscall
	
	li $v0,readStr
	la $a0,str1
	la $a1,STR_MAX_SIZE
	syscall
	
	li $v0,printStr
	la $a0,prt3
	syscall
	
	li $v0,readStr
	la $a0,str2
	la $a1,STR_MAX_SIZE
	syscall
	
	li $v0,printStr
	la $a0,prt3
	syscall
	
	li $v0,printStr
	la $a0,prt2
	syscall
	
	la $a0,str1
	addi	$sp,$sp,-4	
	sw	$ra,0($sp)	# $sp = $ra
	jal strlen
	lw	$ra,0($sp)	# $ra = $sp
	addi	$sp,$sp,4	# pop($ra)
	move $s0,$v0
	
	la $a0,str2
	addi	$sp,$sp,-4	
	sw	$ra,0($sp)	# $sp = $ra
	jal strlen
	lw	$ra,0($sp)	# $ra = $sp
	addi	$sp,$sp,4	# pop($ra)
	move $s1,$v0
	
	li $v0,printInt
	move $a0,$s0
	li   $a1,10
	syscall
	
	li $v0,printStr
	la $a0,prt3
	syscall
	
	li $v0,printInt
	move $a0,$s1
	li   $a1,10
	syscall
	
	la $a0,str3
	la $a1,str1
	addi	$sp,$sp,-4	
	sw	$ra,0($sp)	# $sp = $ra
	jal strcpy
	lw	$ra,0($sp)	# $ra = $sp
	addi	$sp,$sp,4	# pop($ra)
	
	la $a0,str3
	la $a1,str2
	addi	$sp,$sp,-4	
	sw	$ra,0($sp)	# $sp = $ra
	jal strcat
	lw	$ra,0($sp)	# $ra = $sp
	addi	$sp,$sp,4	# pop($ra)
	move $a0,$v0
	li $v0,printStr
	syscall
	
	la $a0,str1
	la $a1,str2
	addi	$sp,$sp,-4	
	sw	$ra,0($sp)	# $sp = $ra
	jal strcmp
	lw	$ra,0($sp)	# $ra = $sp
	addi	$sp,$sp,4	# pop($ra)
	move $a0,$v0
	li $v0,printInt10
	syscall
	li $v0,0
	jr	$ra
	


##################################################################################################
#strlen(char *s)
strlen:		li $t1,0

for_len:	lb	$t0,0($a0)	# while(*s++ != '\0')
		beq	$t0,0,endf	# {
		addiu	$a0,$a0,1	#
		addi	$t1,$t1,1	# 	len ++
		j	for_len		# }
endf:		move	$v0,$t1		# return len;
		jr	$ra		#	
		
###################################################################################################
#strcat(char *dst, char *src)

strcat:	addi $sp,$sp,-16
	sw $ra,0($sp)
	sw $s0,4($sp)
	sw $s1,8($sp)
	sw $s2,12($sp)

	move $s0,$a0
	move $s1,$a1
	move $s2,$a0	
strcat_for:
	lb $t0,0($s0)
	beq $t0,0,strcat_endfor
	addi $s0,$s0,1
	j strcat_for
strcat_endfor:
	move $a0,$s0
	move $a1,$s1
	jal strcpy
	
	move $v0,$s2
	lw $ra,0($sp)
	lw $s0,4($sp)
	lw $s1,8($sp)
	lw $s2,12($sp)
	jr $ra

####################################################################################################
#strcpy(char *dst, char*src)

strcpy:		move $v0,$a0

for_copy:	lb $t0,0($a1)
		sb $t0,0($a0)
		beq $t0,0,strcpy_endfor

		addi $a0,$a0,1
		addi $a1,$a1,1
		j for_copy
strcpy_endfor:	jr $ra	

#################################################################################################
# strcmp(char *s1, char *s2)

strcmp:
strcmpfor:	lb $t0,0($a0)
		lb $t1,0($a1)
		bne $t0,$t1,strcmp_endfor
	
		beq $t0,0,strcmp_endfor
		addi $a0,$a0,1
		addi $a1,$a1,1
		j strcmpfor

strcmp_endfor:	sub $v0,$t1,$t0
		jr $ra
