.data
   .byte 0x83 0xa1
data_label:
   .word 0x98a743ef
.text
   add a2, a0, a1
   or a4, a3, t0
text_label:
   jr t1
   slt t2, t3, s0
   sltu s1, s2, s3
   sll sp, ra, a2
