.data
num: .space 12

.text
max:
	lw $s0, -4($sp)
	lw $s1, -8($sp)
	sw $s0, -12($sp)
	lw $s0, -12($sp)
	li $s1, 0
	beq $s0, $s1, _L6
	j _L3
_L6:
	j _L4
_L3:
	lw $v0, -4($sp)
	jr $ra
	j _L5
_L4:
	lw $v0, -8($sp)
	jr $ra
_L5:
	jr $ra