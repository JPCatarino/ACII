    .equ    SFR_BASE_HI, 0xBF88
    
    .equ    TRISE, 0x6100
    .equ    PORTE, 0x6110
    .equ    LATE,  0x6120

    .equ    TRISB, 0x6040
	.equ    PORTB, 0x6050

    .data
	.text
	.globl	main


main:
    lui     $t1,SFR_BASE_HI
    lw		$t2, TRISE($t1)
    andi    $t2, $t2, 0xFFF0
    sw		$t2, TRISE($t1)

    lw      $t2, TRISB($t1)
    andi    $t2,$t2,0x000F
    sw      $t2,TRISB($t1)

while:
    lw      $t2,PORTB($t1)
    andi    $t0,$t2,0x0008
    andi    $t2,$t2,0x0007
    li      $t3,0
    beq		$t0, $0, posva_for
    li      $t3,7

negva_for:    
    bgt     $t2,3,while
    beq     $t2,$0,sendToLed_neg
    addi    $t3,$t3,-1
    addi    $t2,$t2,-1
    j       negva_for


    

posva_for:
    bgt		$t2, 3, while	
    beq     $t2, $0,sendToLed_pos
    sll     $t3, $t3, 1
    addi    $t3, $t3, 1
    addi    $t2, $t2,-1
    j		posva_for

    

sendToLed_pos:
    sw      $t3, LATE($t1)
    j		while

sendToLed_neg:
    add     $t0, $t0, $t3
    sw      $t0, LATE($t1)
    j       while
    
    jr		$ra					
    
    
        
    