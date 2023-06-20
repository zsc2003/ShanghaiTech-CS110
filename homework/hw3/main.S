.import input.S
.import func.S

.text  # code starts here

.globl main
main:
	# load the address of a0 into str
	la a0 str
	# call your function
   	jal LPS
	# print the result
	mv a1 a0
   	li a0 1
	ecall
	# exit
  	li a0 10
   	ecall


