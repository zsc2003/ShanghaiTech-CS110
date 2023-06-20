.data
test_input: .word 3 6 7 8

.text
main:
	add t0, x0, x0
	addi t1, x0, 4
	la t2, test_input
main_loop:
	beq t0, t1, main_exit
	slli t3, t0, 2
	add t4, t2, t3
	lw a0, 0(t4) # input data

	addi sp, sp, -20
	sw t0, 0(sp)
	sw t1, 4(sp)
	sw t2, 8(sp)
	sw t3, 12(sp)
	sw t4, 16(sp)

	jal ra, factorial

	lw t0, 0(sp)
	lw t1, 4(sp)
	lw t2, 8(sp)
	lw t3, 12(sp)
	lw t4, 16(sp)
	addi sp, sp, 20

	addi a1, a0, 0 # use a0 to store answer
	addi a0, x0, 1
	ecall # Print Result using a1
	addi a1, x0, 32
	addi a0, x0, 11
	ecall # print ' ' using a1
	
	addi t0, t0, 1
	jal x0, main_loop
main_exit:  
	addi a0, x0, 10
	ecall # Exit


# calculate factorial
factorial:
	add a1, x0, a0
	addi a2, x0, 1
	bne a0, x0, fac_loop
	jalr x0, x1, 0 # Exit

fac_loop:
#	beq a2, a1, fac_exit
	mul a0, a0, a2
	addi a2, a2, 1
	bne a2, a1, fac_loop
	jalr x0, x1, 0 # Exit

# fac_exit:
#	addi a0, x0, 10
#	ecall # Exit
#	ecall # Exit