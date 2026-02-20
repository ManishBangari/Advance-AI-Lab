from z3 import *

x = Int('x')
y = Int('y')
print(simplify(x + y + 2*x + 1))
print(simplify(x <= y + x + 2))
print(simplify(And(x + 1 >= 3,
x**2 + y**2 + 3*x**2 -2*y**2>= 2)))