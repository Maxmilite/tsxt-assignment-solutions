#include <cmath>
#include <iostream>

double f(double x) {
    return x * exp(-x) + pow(x, 3) + 1;
}

int main() {
    double p0, p1, TOL;
    int N0;
    std::cin >> p0 >> p1 >> TOL >> N0;
    
    int i = 2;
    double q0 = f(p0);
    double q1 = f(p1);
    
    while (i <= N0) {
        double p = p1 - q1 * (p1 - p0) / (q1 - q0);
        if (std::abs(p - p1) < TOL) {
            std::cout << p << std::endl;
            return 0;
        }
        i = i + 1;
        double q = f(p);
        if (q * q1 < 0) {
            p0 = p;
            q0 = q;
        }
        p1 = p;
        q1 = q;
    }

    std::cout << "Method failed after N0 iterations, N0 = " << N0 << std::endl;
}