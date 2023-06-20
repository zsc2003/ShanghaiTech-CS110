		addi t0, t3, t3					# not a number
label:	jal								# no label
		ori t2, x99, 0xAB				# invalid register
		bne t0, t1, not_exist			# nonexistant label
		addi t1 t2 0x80808080			# number too large

# Can you think of any others?
