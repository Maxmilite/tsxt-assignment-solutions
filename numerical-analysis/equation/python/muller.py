import math

x0, x1, x2, TOL, N = map(float, input().split())

def f(x: float) -> float:
    return x * math.exp(-x) + math.pow(x, 3.0) + 1

h1, h2 = x1 - x0, x2 - x1
delta1, delta2 = (f(x1) - f(x0)) / h1, (f(x2) - f(x1)) / h2
a = (delta2 - delta1) / (h2 + h1)
i = 3

while i <= N:
    b = delta2 + h2 * a
    d = math.sqrt(b * b - 4 * f(x2) * a)
    if abs(b - d) < abs(b + d):
        e = b + d
    else:
        e = b - d
    h = -2 * f(x2) / e
    p = x2 + h
    if abs(h) < TOL:
        print(f"{p}")
        exit(0)
    
    x0, x1, x2 = x1, x2, p
    h1, h2 = x1 - x0, x2 - x1
    delta1, delta2 = (f(x1) - f(x0)) / h1, (f(x2) - f(x1)) / h2
    a = (delta2 - delta1) / (h2 + h1)
    i = i + 1

print(f"Method failed after N iterations, N = {N}")