# DO NOT REMOVE THIS LINE WHEN SUBMITTING
strlen:
    in_loop:
        lb t0, 0(a0)
        addi s1, s1, 1
        addi a0, a0, 1
        bne t0, x0, in_loop
    jr ra

LPS:
    # Input: a0, pointer to string
    # Save the registers
	addi sp,sp,-48
    sw s0,0(sp)
    sw s1,4(sp)
    sw s2,8(sp)
    sw s3,12(sp)
    sw s4,16(sp)
    sw s5,20(sp)
    sw s6,24(sp)
    sw s7,28(sp)
    sw s8,32(sp)
    sw s9,36(sp)
    sw s10,40(sp)
    sw s11,44(sp)


    addi s0, a0, 0 # s0 = static pointer to string
    addi s1, x0, 0
    
    # Get length of string
    addi sp, sp, -4
    sw ra, 0(sp)
    jal strlen

    lw ra, 0(sp)

    addi sp, sp, 4

    addi s1, s1, -1 # set len to s1
    # s2 = Result
    addi s2, x0, 0

    # Init i,j,k
    li t0, 0 # int i = 0
    for_i:
        bge t0, s1,endi # if i >= len, endi
        addi t1, t0, 0 # int j = i
        for_j:
            bge t1, s1, endj # if j >= len, endj
            li t2, 0 # int k = 0
            for_k:
                add t5, t0, t2 # i+k
                sub t6, t1, t2 # j-k
                bge t5, t6, check_update # if i+k >= j-k, check update

                add t3, s0, t0
                add t3, t3, t2 # str+i+k
                lb t3, 0(t3) # Load char at str[i+k]
                add t4, s0, t1
                sub t4, t4, t2 # str+j-k
                lb t4, 0(t4) # Load char at str[j-k]
                addi t2, t2, 1
                bne t3, t4, endk # if str[i+k] != str[j-k], end
                j for_k

            check_update: # This case we try to update the value of res
                sub s3, t1, t0
                addi s3, s3, 1 # s3 = j-i+1

                blt s3, s2, endk # if s3 < s2, end
                addi s2, s3, 0 # s2 = s3
            endk: # Total end of k
                addi t1, t1, 1 # j++
                j for_j


        endj:
            addi t0, t0, 1 # i++
            j for_i

    endi:
        mv a0, s2
        lw s0,0(sp)
        lw s1,4(sp)
        lw s2,8(sp)
        lw s3,12(sp)
        lw s4,16(sp)
        lw s5,20(sp)
        lw s6,24(sp)
        lw s7,28(sp)
        lw s8,32(sp)
        lw s9,36(sp)
        lw s10,40(sp)
        lw s11,44(sp)
        addi sp,sp,48
        
        jr ra
    


    