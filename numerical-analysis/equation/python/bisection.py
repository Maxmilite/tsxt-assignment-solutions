import math

N, TOL, a, b = map(float, input().split())
N = int(N)

def f(x: float) -> float:
    return x * math.exp(-x) + math.pow(x, 3.0) + 1

k, FA = 1, f(a)
while (k <= N):
    c = a + (b - a) / 2
    FC = f(c)
    if (FC == 0 or (b - a) / 2 < TOL):
        print(f"{c}")
        exit(0)
    if (FA * FC < 0):
        b = c
    else:
        a, FA = c, FC # [WARNING] is FA need to be updated? 
    k = k + 1

print(f"Method failed after {N} iterations")
exit(-1)
