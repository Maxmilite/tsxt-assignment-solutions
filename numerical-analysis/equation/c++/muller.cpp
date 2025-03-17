#include <cmath>
#include <iostream>

double f(double x) {
    return x * exp(-x) + pow(x, 3) + 1;
}

int main() {
    double x0, x1, x2, TOL;
    int N;
    std::cin >> x0 >> x1 >> x2 >> TOL >> N;
    
    double h1 = x1 - x0, h2 = x2 - x1;
    double delta1 = (f(x1) - f(x0)) / h1, delta2 = (f(x2) - f(x1)) / h2;
    double a = (delta2 - delta1) / (h2 + h1);
    int i = 3;

    while (i <= N) {
        double b = delta2 + h2 * a;
        double d = sqrt(b * b - 4 * f(x2) * a);
        double e = (abs(b - d) < abs(b + d)) ? b + d : b - d;
        double h = -2 * f(x2) / e;
        double p = x2 + h;
        if (abs(h) < TOL) {
            std::cout << p << std::endl;
            return 0;
        }
        
        x0 = x1;
        x1 = x2;
        x2 = p;
        h1 = x1 - x0;
        h2 = x2 - x1;
        delta1 = (f(x1) - f(x0)) / h1;
        delta2 = (f(x2) - f(x1)) / h2;
        a = (delta2 - delta1) / (h2 + h1);
        i = i + 1;
    }

    std::cout << "Method failed after N iterations, N = " << N << std::endl;

    return 0;
}