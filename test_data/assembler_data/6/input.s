.data

.text
label1:

label2:		beq $t0, $t1, label2 
			bne $t0, $t1, label1
label3:		j label2
			bne $t0, $t1, label3
			
label4:		jal label1

			beq $t0, $t1, label2
            addu $at $v0 $0
            # t
            #e
            #   X
            #       t

############################################################
or $a1 $a2 $0
slt $t0 $t1 $0
sltu $t3 $s0 $0
addu $s2 $s3 $sp
or $ra $zero $0
jr $ra
    #addu $v0, $a0, $a1
or $a2, $a3, $t0
jr $t1
    slt $t2, $t3, $s0
sltu $s1, $s2, $s3
sll $sp, $ra, 3