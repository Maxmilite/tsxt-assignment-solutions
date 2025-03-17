import math

n = int(input())
a = list(map(float, input().split()))
x0 = float(input())
TOL = float(input())
N = int(input())

i, p0 = 1, x0

while i <= N:
    y, z = a[n], a[n]
    for j in range(n - 1, 0, -1):
        y = a[j] + p0 * y
        z = y + p0 * z
    y = a[0] + p0 * y
    p = p0 - y / z
    if abs(p - p0) < TOL:
        print(f'{p}')
        exit(0)
    i = i + 1
    p0 = p

print(f'Method failed')