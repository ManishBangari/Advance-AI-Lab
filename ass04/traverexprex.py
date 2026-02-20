from z3 import *

x = Int('x')
y = Int('y')
e = x + y >= 4
e2 = x - 2*y <= 10
print("Expression e:")
print(" num_args:", e.num_args())
print(" 1st child:", e.arg(0))
print(" 2nd child:", e.arg(1))
print(" children:", e.children())
print(" operator:", e.decl().name())
print("\nExpression e2:")
print(" num_args:", e2.num_args())
print(" 1st child:", e2.arg(0))
print(" 2nd child:", e2.arg(1))
print(" children:", e2.children())
print(" operator:", e2.decl().name())