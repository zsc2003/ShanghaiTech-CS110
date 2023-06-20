### The register representing the variable k.
t0(x5)


### The registers acting as pointers to the source and dest arrays.
source t1(x6)
dest t2(x7)


### The assembly code for the loop found in the C code.
loop:
	slli t3, t0, 2
	add t4, t1, t3
	lw t5, 0(t4)
	beq t5, x0, exit
	add t6, t2, t3
	sw t5, 0(t6)
	addi t0, t0, 1
	jal x0, loop


### How the pointers are manipulated in the assembly code.
source[k]:
lw t5, 0(t4)