### About how to run the project1.1




##### together:

./riscv assembler <input_file> -d <data_file> -s <symbol_table_file> -o <original_file> -b <basic_file> -t <basic_file>

data_file
symbol_table_file
original_file
basic_file
均在输出文件夹中

##### for pass 1:

./riscv assembler-p1 <input_file> -d <data_file> -s <symbol_table_file> -o <original_file>

data_file
symbol_table_file
original_file
均在输出文件夹中

##### for pass 2:

若pass1 unfinshed, 则symbol_table_file为空即可

./riscv assembler-p2 <symbol_table_file> <original_file> -b <basic_file> -t <text_file>

symbol_table_file
original_file
在输入文件夹中

basic_file
text_file
在输出文件夹中