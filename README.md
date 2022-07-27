# Automatic Demonstration of Mathematical Conjectures 

Master thesis conducted under the supervision of Prof. Boigelot at the University of Liege. 

Final document can be found here : https://matheo.uliege.be/bitstream/2268.2/1679/5/Automatic_demonstration_of_mathematical_conjectures.pdf


## About  
This work is focusing on a tool, Walnut (https://cs.uwaterloo.ca/~shallit/walnut.html), developed for automating decision
procedures.
This tool allows to use natural numbers encoded in various numeration
systems. One interesting feature of the tool is the allowance to
add new numeration systems if the addition automaton for this numeration
system is provided. The user must thus provide an automaton accepting the
language L = {(x, y, z)} with (x, y, z) being the encoding of three natural
numbers in the numeration system, and such that the values represented by
x and y added together give the value represented by z.

The aim of this master thesis is to automate the generation of the addition
automaton for the Fibonacci numeration systems. An original solution has
been developed for this problem using automata. This new method is inspired from existing work, ensuring some interesting efficiency properties. 

## Program Execution

Command line to run the program :

./data m alph_max [ a_m-1 ... a_0 ] [ U_m-1 ... U_0 ]

with
- m is the order of the Fibonacci polynomial that will define the numeration system
- alph_max is the alphabet upper bound (operational for 1)
- the a_i are the coefficients of the polynomial, for further generalization, operational for [ 1 ... 1 ]
- the U_i are the first values of the base that can be chosen. U_0 should be equal to 1, U_1 = 2 and the others should satisfy : U_i-1 < U_i <= 2^i 

command for 

Fibonacci (F_2) : ./data 2 1 [ 1 1 ] [ 2 1 ]

Tribonacci (F_3) : ./data 3 1 [ 1 1 1 ] [ 4 2 1 ]
