import math

p0, p1, TOL, N0 = map(float, input().split())

def f(x: float) -> float:
    return x * math.exp(-x) + math.pow(x, 3.0) + 1

i, q0, q1 = 1, f(p0), f(p1)

while i <= N0:
    p = p1 - q1 * (p1 - p0) / (q1 - q0)
    if abs(p - p1) < TOL:
        print(f'{p}')
        exit(0)
    i = i + 1
    p0, q0, p1, q1 = p1, q1, p, f(p)

print(f'Method failed after N0 iterations, N0 = {N0}')