import math

N, TOL, p0 = map(float, input().split())

def g(x: float) -> float:
    return math.exp(-x)

n = 1
while n <= N:
    p = g(p0)
    if abs(p - p0) < TOL:
        print(f"{p}")
        exit(0)
    p0 = p
    n = n + 1

print(f"Method failed after N iterations, N = {N}")