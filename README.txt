Shahria Abeed
sa5680@nyu.edu
Project 2

I have completed this project. It has worked for every test
case I have tried. I also wrote a few test cases myself, 
mainly to see if any of the operations would overwrite 
$0.

My approach in doing this project was just taking each
individual instruction from the memory array, parsing it
and taking note of any important information, such as dst
registers and immediates. The parse_instruc function does 
what I just described.

I altered the load_machine_code function so that I could
access the memoryquantity without having to loop through
the file or memory again. I also altered print_state so
it would continue printing 0's up to 128 instructions.

Afterwards, I went through the E20 manual for any small 
details that effect how the operations act. For example, 
some of the operations use signed numbers or they sign 
extend the numbers. To implement this into my sim, I wrote
the sign_extend function, which takes 7-bit immediates and
extends them where necessary.

The only resources I used for this project were the E20 
manual and notes I took during class about bitwise 
operations. 
