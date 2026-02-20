from z3 import *

n = 6
x = [Bool(f"x_{i}") for i in range(1, n+1)]

weights = [4, 8, 5, 6, 3, 7]
profits = [6, 12, 8, 7, 4, 11]

opt = Optimize()

# Exactly 4 items selected
opt.add(Sum([If(x[i], 1, 0) for i in range(n)]) == 4)

# Weight constraint
total_weight_expr = Sum([If(x[i], weights[i], 0) for i in range(n)])
opt.add(total_weight_expr <= 20)

# Profit objective
total_profit_expr = Sum([If(x[i], profits[i], 0) for i in range(n)])
opt.maximize(total_profit_expr)

print(opt.check())
print(opt.model())
