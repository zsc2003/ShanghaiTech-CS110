#### solution

propagation delay of an adder block   : 45ns
propagation delay of MUX              : 20ns
ropagation delay of a shifters block  :  5ns
register CLK-to-Q delay               : 10ns
setup time                            : 10ns
hold time                             :  5ns


##### move and add
max(MUX+adder, shifter) = max(45 + 20, 5) = 65 ns
delay of the move_and_add part	          : 65ns


##### non pipelined
$T_{clk} \geq setup + t_{cd} + t_{pcq}​$
$T_{clk} \geq 10 + 65 * 4 + 10 = 280ns​$
$(T_{clk})_{min} = 280ns​$
$(f_{clk})_{max} = 3.571 MHz​$

##### pipelined
$T_{clk} \geq setup + t_{cd} + t_{pcq}$
$T_{clk} \geq 10 + 65 + 10 = 85ns$
$(T_{clk})_{min} = 85ns$
$(f_{clk})_{max} = 11.765 MHz​$
