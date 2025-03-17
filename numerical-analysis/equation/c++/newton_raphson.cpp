#include <cmath>
#include <iostream>

double f(double x) {
    return x * exp(-x) + pow(x, 3) + 1;
}

double f_prime(double x) {
    return exp(-x) - x * exp(-x) + 3 * pow(x, 2);
}

int main() {
    double p0, TOL;
    int N;
    std::cin >> p0 >> TOL >> N;

    int i = 1;
    while (i <= N) {
        double p = p0 - f(p0) / f_prime(p0);
        if (abs(p - p0) < TOL) {
            std::cout << p << std::endl;
            return 0;
        }
        p0 = p;
        i = i + 1;
    }

    std::cout << "Method failed after N iterations, N = " << N << std::endl;

    return 0;
}