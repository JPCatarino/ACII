    .equ    SFR_BASE_HI, 0xBF88
    
    .equ    TRISE, 0x6100
    .equ    PORTE, 0x6110
    .equ    LATE,  0x6120

    .equ    TRISB, 0x6040
	.equ    PORTB, 0x6050

    .equ PUT_CHAR, 3
	.equ PRINT_INT, 6

    .equ RESET_CORE_TIMER,12
	.equ READ_CORE_TIMER,11

    .data
	.text
	.globl	main

main:
    addu    $sp,$sp,-4
    sw		$ra, 0($sp)	

    lui     $s3,SFR_BASE_HI
    lw		$t2, TRISE($s3)		
    andi    $t2,$t2,0xFFF0
    sw      $t2, TRISE($s3)

    lw      $t2, TRISB($s3)
    ori     $t2,$t2,0x0008
    sw      $t2, TRISB($s3)

    li      $s0,5
    li      $s1,0

loop:
    lw      $t0,PORTB($s3)
    andi    $t0,$t0, 0x0008
if:    
#    lw      $t2, LATE($s3)
#    andi    $t2,$t2,0xFFF0
#   or      $t2,$t2,$s0
    li	$a0,'n'
	li	$v0,PUT_CHAR
	syscall
    sw      $s0, LATE($s3)  
    beq     $t0,$0,lowspeed
    

highspeed:


#   li	$a0,'+'
#	li	$v0,PUT_CHAR
#	syscall
    li      $a0,800
    jal		delay				# jump to delay and save position to $ra
    j		end_if				# jump to end_if
    


lowspeed:

#    li	$a0,'-'
#	li	$v0,PUT_CHAR
#	syscall
    li      $a0,100
    jal		delay	
    j		end_if				# jump to end_if
    

end_if:
    beq     $s1,0,subtract
    beq     $s1,1,add_one


flag:   
    beq     $s1,0,set_pos
    beq     $s1,1,set_neg

set_neg:
    addi    $s0,$s0,-1
    li      $s1,0
    j		loop				# jump to loop
    

set_pos:
    addi    $s0,$s0,1
    li      $s1,1
    j		loop				# jump to loop
    

subtract:
    beq     $s0,0,flag
#    li	$a0,'-'
#	li	$v0,PUT_CHAR
#	syscall
#	move	$a0,$s0
#	li	$a1,10
#	li	$v0,PRINT_INT
#	syscall
    addi    $s0,$s0,-1
    j		loop				# jump to loop
    

add_one:
    beq     $s0,5,flag
 #   li	$a0,'+'
#	li	$v0,PUT_CHAR
#	syscall
#	move	$a0,$s0
#	li	$a1,10
#	li	$v0,PRINT_INT
#	syscall
    addi    $s0,$s0,1
    j		loop				# jump to loop

    lw      $ra,0($sp)
    addu    $sp,$sp,4
    jr      $ra
       
    
    
     
    


###############################################################
# delay(ms){}

delay:

for:    
    ble     $a0,0,endfor
    li		$v0,RESET_CORE_TIMER
    syscall

whild:
    li      $v0,READ_CORE_TIMER
    syscall
    blt     $v0,20000,whild
    addi	$a0, $a0, -1
    j       for

endfor: 
    jr      $ra
    		
    