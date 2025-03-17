#include <bits/stdc++.h>

int main() {
    int n;
    std::cin >> n;
    std::vector<double> x(n), y(n);
    for (int i = 0; i < n; i++) {
        std::cin >> x[i] >> y[i];
    }
    double x0, y0 = 0;
    std::cin >> x0;
    for (int i = 0; i < n; i++) {
        double p = 1;
        for (int j = 0; j < n; j++) {
            if (i != j) {
                p *= (x0 - x[j]) / (x[i] - x[j]);
            }
        }
        y0 += y[i] * p;
    }
    std::cout << y0 << std::endl;
}