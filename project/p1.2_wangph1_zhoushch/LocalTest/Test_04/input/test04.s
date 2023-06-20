.data
test_input: .word 3 6 7 8

.text
main:
	add t0, x0, x0 
	addi t1, x0, 4 # t1 = 4
	la t2, test_input # Location
main_loop: 
	beq t0, t1, main_exit # While t0 != t1
	slli t3, t0, 2 # t3 = t0 * 4
	add t4, t2, t3 # Source + Offset
	lw a0, 0(t4) # Load n in a0

	addi sp, sp, -20 # Save data in stack
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
	addi sp, sp, 20 # Load data from stack

	addi a1, a0, 0 # Print
	addi a0, x0, 1
	ecall # Print Result
	addi a1, x0, 32
	addi a0, x0, 11
	ecall
	
	addi t0, t0, 1
	jal x0, main_loop
main_exit:  
	addi a0, x0, 10
	ecall # Exit

factorial:
    addi s2, x0, 1 # result = 1
loop:
	beq a0, x0, exit
    mul s2, s2, a0 
    addi a0, a0, -1
    jal x0, loop
exit:
    addi a0, s2, 0
    jr ra 
