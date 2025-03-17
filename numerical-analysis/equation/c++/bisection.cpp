#include <cmath>
#include <iostream>

double f(double x) {
    return x * exp(-x) + pow(x, 3) + 1;
}

int main() {
    int N;
    double TOL, a, b;
    std::cin >> N >> TOL >> a >> b;
    
    int k = 1;
    double FA = f(a);

    while (k <= N) {
        double c = a + (b - a) / 2;
        double FC = f(c);
        if (FC == 0 || (b - a) / 2 < TOL) {
            std::cout << c << std::endl;
            return 0;
        }
        if (FA * FC < 0) {
            b = c;
        } else {
            a = c;
            FA = FC;
        }
        k = k + 1;
    }

    std::cout << "Method failed after " << N << " iterations" << std::endl;

    return 0;
}