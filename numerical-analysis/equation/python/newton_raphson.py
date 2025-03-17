import math

p0, TOL, N = map(float, input().split())

def f(x: float) -> float:
    return x * math.exp(-x) + math.pow(x, 3.0) + 1

def f_prime(x: float) -> float:
    return math.exp(-x) - x * math.exp(-x) + 3 * math.pow(x, 2.0)

i = 1
while i <= N:
    p = p0 - f(p0) / f_prime(p0)
    if abs(p - p0) < TOL:
        print(f"{p}")
        exit(0)
    p0 = p
    i = i + 1

print(f"Method failed after N iterations, N = {N}")