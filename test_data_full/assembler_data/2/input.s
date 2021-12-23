.data
a: .space 4
_a: .space 8
__a: .space 16
_a__: .space 32
_123: .space 64#a_23: .space 6
a_23: .space 128  # a_23: .space 6 
a1: .space 256 
    # This is
#.data comment.
_a_1: .space 512
a_: .space 1024
a0: .space 2048

.text
la $t1, a
    la $t2, _a
la $t3, __a
    la $t4, _a__
la $t5, _123
la $t6, a_23
    la $t7, a1
la $t8, a0


addu $t1, $t2, $t2
# Comment here
	addu $v0, $a0, $a1
	or $a2, $a3, $t0			# comments # comments
	
	jr $t1
	slt $t2, $t3, $s0
	
	# Comment here
	sltu $s1, $s2, $s3	# some more comments here
	#sll $sp, $ra, 3
    sw $s1, 0($t8)
    sw $s1, 4($t8)
    sw $s1, 8($t8)
    sw $s1, 16($t8)
    sw $s1, 128($t8)
    sw $s1, 1024($t8)
    lb $10, 4($t8)
    lw $v0, 2000($t8)

	
#################### hi #########
	ori $0 $0 0x0

