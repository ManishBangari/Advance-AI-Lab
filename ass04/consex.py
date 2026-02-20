from z3 import *

x = Int('x')
y = Int('y')
s = Solver()
s.add(x + y == 10)
s.add(x >= 0, y >= 0)
s.add(x % 2 == 0)
if s.check() == sat:
    print(s.model())