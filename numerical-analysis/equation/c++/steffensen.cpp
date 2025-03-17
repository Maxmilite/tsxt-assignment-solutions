#include <cmath>
#include <iostream>

double g(double x) {
    return std::exp(-x) - x;
}

int main() {
    double p0, TOL, N0;
    std::cin >> p0 >> TOL >> N0;

    int i = 1;
    while (i <= N0) {
        double p1 = g(p0);
        double p2 = g(p1);
        double p = p0 - std::pow(p1 - p0, 2.0) / (p2 - 2 * p1 + p0);
        if (std::abs(p - p0) < TOL) {
            std::cout << p << std::endl;
            return 0;
        }
        i = i + 1;
        p0 = p;
    }

    std::cout << "Method failed after N0 iterations, N0 = " << N0 << std::endl;
    return 1;
}