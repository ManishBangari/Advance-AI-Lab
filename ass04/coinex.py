from z3 import *

coins = [1, 2, 4, 7, 8, 10]
n = len(coins)

x = []
for i in range(n):
    x.append(Int(f"x_{i}"))

opt = Optimize()

constraints = []

for i in range(n):
    constraints.append(Or(x[i] == 0, x[i] == 1))

total_value = 0
for i in range(n):
    total_value += coins[i] * x[i]

constraints.append(total_value == 15)

opt.add(constraints)

num_coins = Sum(x)
opt.minimize(num_coins)

if opt.check() == sat:
    m = opt.model()
    selected = [coins[i] for i in range(n) if m[x[i]].as_long() == 1]
    print(selected)
