from z3 import *

p = Int('p')
q = Int('q')
opt = Optimize()
opt.add(2*p + 4*q <= 12)
opt.add(3*p + q <= 9)
opt.add(p >= 0, q >= 0)
opt.maximize(5*p + 3*q)
if opt.check() == sat:
    print(opt.model())