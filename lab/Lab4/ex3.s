.data
n: .word 3 # number of test data
data: # input, answer
.word 0x12345678, 0x1E6A2C48
.word 0x71C924BF, 0xFD24938E
.word 0x19260817, 0xE8106498

.text
# === main and helper functions ===
# You don't need to understand these, but reading them may be useful
main:
    la t0, n # t0 = &n
    lw s0, 0(t0) # s0 = *t0 (s0 = 3)
    la s1, data # s1 = data
    main_loop: # run each of the n tests
        beq s0, zero, main_loop_end # s0 == 0
        lw a0, 0(s1) # a0 = data[i] (i=0,2,4)
        jal print_hex_and_space # print input
        lw a0, 4(s1) # a0 = data[i+1]
        jal print_hex_and_space # print answer
        lw a0, 0(s1) # a0 = data[i]
        jal bitrev1
        jal print_hex_and_space # print result of bitrev1
        lw a0, 0(s1) # a0 = data[i]
        jal bitrev2
        jal print_hex_and_space # print result of bitrev2
        jal print_newline
        addi s0, s0, -1 # s0--
        addi s1, s1, 8 # i+=2
        j main_loop
    main_loop_end:
    li a0, 10 # end the program
    ecall # exit

print_hex_and_space:
    mv a1, a0
    li a0, 34
    ecall
    li a1, ' '
    li a0, 11
    ecall
    ret

print_newline:
    li a1, '\n'
    li a0, 11
    ecall
    ret

# === The first version ===
# Reverse the bits in a0 with a loop
bitrev1:
    ### TODO: YOUR CODE HERE ###
    addi sp, sp, -4
    sw ra, 0(sp) # store register point
    
    addi a0, x0, 0 # a0 0
    lw a1, 0(s1) # a1 = data[i] (i=0,2,4)
    addi a3, x0, 0 # a3 : bits
    addi a4, x0, 32
    reverse_loop:
        beq a3, a4, reverse_loop_end
        slli a0, a0, 1
        andi a2, a1, 1
        add a0, a0, a2
        srli a1, a1, 1
        addi a3, a3, 1
        jal reverse_loop

    reverse_loop_end:
        # jal print_hex_and_space # print input
        lw ra, 0(sp)
        addi sp, sp, 4
        ret

# === The second version ===
# Reverse the bits in a0. Only use li, and, or, slli, srli!
bitrev2:
    ### TODO: YOUR CODE HERE ###
    
    # step 1 k = 32
    li a2, 0xFFFF0000 # 1111 1111 1111 1111 0000 0000 0000 0000
    li a3, 0x0000FFFF # 0000 0000 0000 0000 1111 1111 1111 1111
    and a2, a2, a0
    and a3, a3, a0
    srli a2, a2, 16
    slli a3, a3, 16
    or a0, a2, a3

    # step 2 k = 16
    li a2, 0xFF00FF00 # 1111 1111 0000 0000 1111 1111 0000 0000
    li a3, 0x00FF00FF # 0000 0000 1111 1111 0000 0000 1111 1111
    and a2, a2, a0
    and a3, a3, a0
    srli a2, a2, 8
    slli a3, a3, 8
    or a0, a2, a3

    # step 3 k = 8
    li a2, 0xF0F0F0F0 # 1111 0000 1111 0000 1111 0000 1111 0000
    li a3, 0x0F0F0F0F # 0000 1111 0000 1111 0000 1111 0000 1111
    and a2, a2, a0
    and a3, a3, a0
    srli a2, a2, 4
    slli a3, a3, 4
    or a0, a2, a3

    # step 4 k = 4
    li a2, 0xCCCCCCC # 1100 1100 1100 1100 1100 1100 1100 1100
    li a3, 0x3333333 # 0011 0011 0011 0011 0011 0011 0011 0011
    and a2, a2, a0
    and a3, a3, a0
    srli a2, a2, 2
    slli a3, a3, 2
    or a0, a2, a3

    # step 5 k = 2
    li a2, 0xAAAAAAA # 1010 1010 1010 1010 1010 1010 1010 1010
    li a3, 0x5555555 # 0101 0101 0101 0101 0101 0101 0101 0101
    and a2, a2, a0
    and a3, a3, a0
    srli a2, a2, 1
    slli a3, a3, 1
    or a0, a2, a3

    ret
