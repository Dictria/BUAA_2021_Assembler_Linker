.data

.text
main:
    la $v1, num
	addiu $a3, $sp, 0
	sw $ra, 0($a3)
	lw $a2, 0($v1)
	sw $s0, -4($a3)
	lw $a2, 4($v1)
	sw $s0, -8($a3)
	lw $a2, 8($v1)
	sw $s0, -12($a3)
	move $sp, $a3
	jal sort
	lw $ra, 0($sp)
	addiu $sp, $sp, 0
	sw $v0, 0($sp)