### What caused the errors in simple_fn, naive_pow, and inc_arr that were reported by the Venus CC checker?

simple_fn: move value in a0 to t0, but useless as a0's value won't be used anyway

naive_pow: use the register of s0, but is not saved in the prologue and not load when the function ends

inc_arr: use the register of s0, s1, but they are not saved in the prologue and not load when the function ends

### In RISC-V, we call functions by jumping to them and storing the return address in the ra register. Does calling convention apply to the jumps to the naive_pow_loop or naive_pow_end labels?

Actually no, as there for naive_pow_loop call itself at the end of the function, so not necessary to store the address, as for the naive_pow_end, it is not a function, so not necessary to store the address.

### Why do we need to store ra in the prologue for inc_arr, but not in any other function?

As among all these functions, only inc_arr calls other functions. So we need to store ra in the prologue for inc_arr. Or we can also store ra in the prologue for all functions, but it is not necessary.

### Why wasn't the calling convention error in helper_fn reported by the CC checker? (Hint: it's mentioned above in the exercise instructions.)

As the instruction says, the calling convention is not checked for functions that are not in .globl directive. So helper_fn is not checked and thus no error is reported.