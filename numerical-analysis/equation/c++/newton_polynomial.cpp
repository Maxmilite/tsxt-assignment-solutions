#include <cmath>
#include <iostream>
#include <vector>

int main() {
    int n;
    std::cin >> n;
    std::vector<double> a(n + 1);
    for (int i = 0; i <= n; i++) {
        std::cin >> a[i];
    }
    double x0, TOL;
    int N;
    std::cin >> x0 >> TOL >> N;

    int i = 1;
    double p0 = x0;

    while (i <= N) {
        double y = a[n], z = a[n];
        for (int j = n - 1; j > 0; j--) {
            y = a[j] + p0 * y;
            z = y + p0 * z;
        }
        y = a[0] + p0 * y;
        double p = p0 - y / z;
        if (std::abs(p - p0) < TOL) {
            std::cout << p << std::endl;
            return 0;
        }
        i = i + 1;
        p0 = p;
    }

    std::cout << "Method failed" << std::endl;

    return 0;
}