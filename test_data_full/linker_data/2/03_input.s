.data

.text:
min:
	lw $s0, -4($sp)
	lw $s1, -8($sp)
	slt $s0, $s0, $s1
	sw $s0, -12($sp)
	lw $s0, -12($sp)
	li $s1, 0
	beq $s0, $s1, _L2
	j _L0
_L2:
	j _L1
_L0:
	lw $v0, -4($sp)
	jr $ra
_L1:
	lw $v0, -8($sp)
	jr $ra
