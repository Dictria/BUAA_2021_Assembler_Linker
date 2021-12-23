.data

.text
li $s3, 0
li $s4, 10
li $s5, 0x10
li $s6, 0x12345678
li $s6, 305419896
# li
# move
#sw
#next instruction
addiu $t0, $t1, 1000
addiu $t0, $t1, -56
ori $t3, $a0, 30000
ori $t3, $a0, 0xD
lui $a0, 31
    #            G
move $t3, $a0
lui $a3, 0x3F3F
lb $s0, 0($a2)
lbu $s0, 128($a2)
lw $s1 156($a3)
sb $s2, -35($t2)
sw $s3 -999($t3)
end: