from z3 import *

x = Int('x')
s = Solver()
s.add(x >= 0)
s.push()
s.add(x > 10)
if s.check() == sat:
    print("SAT case 1:",
s.model())
s.pop()
s.add(x < -2)
if s.check() == sat:
    print("SAT case 2:",
s.model())