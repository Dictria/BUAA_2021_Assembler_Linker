.data
array: .space 16
# end of the data segment

.text
li $v1, 268500992
	li $s0, 0
	sw $s0, 4400($v1)
j main
compare:
	lw $s0, 4400($v1)
	li $s1, 1
	addu $s0, $s0, $s1
	sw $s0, -12($sp)
	lw $s0, -12($sp)
	sw $s0, 4400($v1)
	lw $s0, -4($sp)
	lw $s1, -8($sp)
	sw $s0, -16($sp)
	lw $s0, -16($sp)
    la $17, array
	li $s1, 0
	beq $s0, $s1, L3
	j L0
L3:
	j L1
L0:
	li $v0, 0
	jr $ra
	j L2
L1:
	lw $s0, -4($sp)
	lw $s1, -8($sp)
	sltu $s0, $s0, $s1
	sw $s0, -20($sp)
	lw $s0, -20($sp)
	li $s1, 0
	beq $s0, $s1, L7
	j L4
L7:
	j L5
L4:
	li $v0, 1
	jr $ra
	j L6
L5:
	li $v0, -1
	jr $ra
L6:
L2:
	li $v0, -2333
	jr $ra
	jr $ra