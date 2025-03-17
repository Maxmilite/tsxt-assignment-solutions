import math

n = int(input())
a = list(map(float, input().split()))
x0 = float(input())

y, z = a[n], a[n]

for j in range(n - 1, 0, -1):
    y = a[j] + x0 * y
    z = y + x0 * z

y = a[0] + x0 * y

print(f'{y} {z}')

