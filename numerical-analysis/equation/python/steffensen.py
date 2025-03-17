import math

p0, TOL, N0 = map(float, input().split())

def g(x: float) -> float:
    return math.exp(-x) - x

i = 1
while i <= N0:
    p1 = g(p0)
    p2 = g(p1)
    p = p0 - math.pow(p1 - p0, 2.0) / (p2 - 2 * p1 + p0)
    if abs(p - p0) < TOL:
        print(f"{p}")
        exit(0)
    i = i + 1
    p0 = p

print(f"Method failed after N0 iterations, N0 = {N0}")