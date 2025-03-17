#include <cmath>
#include <iostream>

double g(double x) {
    return exp(-x);
}

int main() {
    int N;
    double TOL, p0;
    std::cin >> N >> TOL >> p0;
    
    int n = 1;
    while (n <= N) {
        double p = g(p0);
        if (std::abs(p - p0) < TOL) {
            std::cout << p << std::endl;
            return 0;
        }
        p0 = p;
        n = n + 1;
    }

    std::cout << "Method failed after N iterations, N = " << N << std::endl;

    return 0;
}