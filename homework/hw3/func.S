# DO NOT REMOVE THIS LINE WHEN SUBMITTING
.globl LPS

LPS:
# YOUR CODE HERE
    # -----------------------testing output--------------------------
    
    addi sp, sp, -4
    sw ra, 0(sp) # ra that come from main
    
    addi a1, x0, 0 # the length of the string
    li t0, 0 # initialize the boundary '\0'
    jal ra, strlen # a1 = strlen(string)

    # mv a0, a1 # for debugging
    li a2, 1
    beq a2, a1, ret_main # strlen == 1

    jal ra, outer # a2 is the max length of odd substrings


    ret_main:
        mv a0, a2 # load the answer
        lw ra, 0(sp)
        addi sp, sp, 4
        jr ra # go back to main

strlen:
    addi sp, sp, -4 # save ra
    sw ra, 0(sp)
    
    strlen_loop:
        add t1, a1, a0 # t1 bit of the string
        lb t2, 0(t1) # t2 = *t1 (str[a1])
        beq t2, t0, end_strlen 
        addi a1, a1, 1 # a1++
        j strlen_loop # for loop

    end_strlen:
        lw ra, 0(sp) # load ra
        addi sp, sp, 4
        ret # return to LPS

outer:
    addi sp, sp, -4 # save ra
    sw ra, 0(sp)

    li t0, 0 # t0 : loop parameter(i)
    addi a7, a1, -1

    outer_loop:
        beq t0, a7, end_outer # i == len - 1
        add t1, t0, a0
        lb t2, 0(t1) # t2 : s[i]
        addi t1, t1, 1 
        lb t3, 0(t1) # t3 : s[i + 1]
        addi t1, t1, -2 
        lb t4, 0(t1) # t4 : s[i - 1]

        jal ra, even 
        jal ra, max # ans = max(ans, num)
        
        jal ra, odd
        jal ra, max # ans = max(ans, num)

        addi t0, t0, 1 # i++
        j outer_loop

    end_outer:
        lw ra, 0(sp) # load ra
        addi sp, sp, 4
        ret # return to LPS

even:
    addi sp, sp, -4 # save ra
    sw ra, 0(sp)

    li a3, 0 # a3 : num = 0
    li a4, 0 # a4 : offset = 0
    
    bne t2, t3, end_even # s[i] != s[i+1]
    
    even_loop:
        sub a5, t0, a4 # a5: i - offset
        blt a5, x0, end_even # i - offset < 0

        add a6, t0, a4
        addi a6, a6, 1 # a6 : i + offset + 1
        bge a6, a1, end_even # i + offset + 1 >= len

        add a5, a5, a0 # s[i - offset]'s address 
        add a6, a6, a0 # s[i + offset + 1]'s address 

        lb t6, 0(a5) # t6 : s[i - offset]
        lb t5, 0(a6) # t5 : s[i + offset + 1]

        bne t6, t5, end_even # s[i - offset] != s[i + offset + 1]

        addi a3, a3, 2 # num += 2
        addi a4, a4, 1 # offset++
        j even_loop

    end_even:
        lw ra, 0(sp) # load ra
        addi sp, sp, 4
        ret # return to outer

odd:
    addi sp, sp, -4 # save ra
    sw ra, 0(sp)

    li a3, 1 # a3 : num = 1
    li a4, 1 # a4 : offset = 1
    
    bne t4, t3, end_odd # s[i-1] != s[i+1]
    
    odd_loop:
        sub a5, t0, a4 # a5: i - offset
        blt a5, x0, end_odd # i - offset < 0

        add a6, t0, a4 # a6 : i + offset
        bge a6, a1, end_odd # i + offset >= len

        add a5, a5, a0 # s[i - offset]'s address 
        add a6, a6, a0 # s[i + offset]'s address 

        lb t6, 0(a5) # t6 : s[i - offset]
        lb t5, 0(a6) # t5 : s[i + offset]

        bne t6, t5, end_odd # s[i - offset] != s[i + offset]

        addi a3, a3, 2 # num += 2
        addi a4, a4, 1 # offset++
        j odd_loop

    end_odd:
        lw ra, 0(sp) # load ra
        addi sp, sp, 4
        ret # return to outer

max:
    addi sp, sp, -4 # save ra
    sw ra, 0(sp)
    blt a2, a3, swap # ans < num

    lw ra, 0(sp) # load ra
    addi sp, sp, 4
    ret # return to outer loop

    swap:
        mv a2, a3
        lw ra, 0(sp) # load ra
        addi sp, sp, 4
        ret # return to outer loop

