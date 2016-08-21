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
