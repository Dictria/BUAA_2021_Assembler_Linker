.data

.text
sort:
	addiu $a3, $sp, -28
	sw $ra, 0($a3)
	lw $a2, -4($sp)
	sw $s0, -4($a3)
	lw $a2, -8($sp)
	sw $s0, -8($a3)
	move $sp, $a3
	jal min
	lw $ra, 0($sp)
	addiu $sp, $sp, 28
	sw $v0, -28($sp)
	addiu $a3, $sp, -32
	sw $ra, 0($a3)
	lw $a2, -28($sp)
	sw $s0, -4($a3)
	lw $a2, -12($sp)
	sw $s0, -8($a3)
	move $sp, $a3
	jal min
	lw $ra, 0($sp)
	addiu $sp, $sp, 32
	sw $v0, -32($sp)
	lw $s0, -32($sp)
	sw $s0, -16($sp)
	addiu $a3, $sp, -36
	sw $ra, 0($a3)
	lw $a2, -4($sp)
	sw $s0, -4($a3)
	lw $a2, -8($sp)
	sw $s0, -8($a3)
	move $sp, $a3
	jal max
	lw $ra, 0($sp)
	addiu $sp, $sp, 36
	sw $v0, -36($sp)
	addiu $a3, $sp, -40
	sw $ra, 0($a3)
	lw $a2, -36($sp)
	sw $s0, -4($a3)
	lw $a2, -12($sp)
	sw $s0, -8($a3)
	move $sp, $a3
	jal max
	lw $ra, 0($sp)
	addiu $sp, $sp, 40
	sw $v0, -40($sp)
	lw $s0, -40($sp)
	sw $s0, -20($sp)
	jr $ra
