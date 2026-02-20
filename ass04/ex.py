from z3 import *

A = [Int(f'a{i}') for i in range(5)]
B = [Int(f'b{i}') for i in range(5)]
     
A_minus_B = [A[i] - B[i] for i in range(5)]
print(A_minus_B)

A_eq_B = [A[i] == B[i] for i in range(5)]
print(And(A_eq_B))
