#include <bits/stdc++.h>

int main() {
    int n;
    std::cin >> n;
    std::vector<double> x(n), y(n);
    for (int i = 0; i < n; i++) {
        std::cin >> x[i] >> y[i];
    }
    std::vector<std::vector<double>> f(n, std::vector<double>(n));
    for (int i = 0; i < n; i++) {
        f[i][0] = y[i];
    }
    for (int i = 1; i < n; i++) {
        for (int j = 1; j <= i; j++) {
            f[i][j] = (f[i][j - 1] - f[i - 1][j - 1]) / (x[i] - x[i - j]);
        }
    }
    for (int i = 0; i < n; ++i) {
        std::cout << f[i][i] << " \n"[i == n - 1];
    }
}