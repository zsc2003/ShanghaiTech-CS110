.data
	str1:	.string "wphgg"
	str2:	.string "tql"

.section .text
.globl draw
.type draw, @function

draw:
	# YOUR CODE HERE

	addi 	sp,	sp,	-4
	sw 		ra,	0(sp)

	# draw a circle
	# LCD_DrawCircle(25, 40, 20, BLUE);
	li 		a0,	25
	li 		a1, 40
	li 		a2,	20
	li 		a3, 0x001F
	jal 	LCD_DrawCircle

	# draw a square
	# LCD_DrawRectangle(50, 20, 90, 60, RED);
	li		a0,	50
	li 		a1,	20
	li 		a2,	90
	li 		a3,	60
	li 		a4, 0xF800
	jal 	LCD_DrawRectangle

	# print a string
	# LCD_ShowString(str1, 110, 20, t0, GREEN);
	la 		t0,	str1  # a pointer to char* "wphgg"
	li 		a0,	110
	li 		a1,	20
	mv 		a2,	t0
	li 		a3,	0x07E0
	jal 	LCD_ShowString

	# print a string
	# LCD_ShowString(str2, 110, 40, t0, GREEN);
	la 		t0,	str2  # a pointer to char* "tql"
	li 		a0,	110
	li 		a1,	40
	mv 		a2,	t0
	li 		a3,	0x07E0
	jal 	LCD_ShowString


	# print a num
	# LCD_ShowNum(110, 60, 100, 3, YELLOW);
	li		a0,	110
	li 		a1,	60
	li 		a2,	100
	li 		a3,	3
	li 		a4, 0xFFE0
	jal 	LCD_ShowNum

	lw 		ra,	0(sp)
	addi 	sp,	sp,	4
	ret 
