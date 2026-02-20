from z3 import *
a, b = Bools('a b')
expr = And(Not(a), b, And(a, b))
simplified = simplify(expr)
print(simplified)