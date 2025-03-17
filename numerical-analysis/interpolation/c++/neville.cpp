#include <bits/stdc++.h>

int main() {
    int n;
    std::cin >> n;
    std::vector<double> x(n), y(n);
    for (int i = 0; i < n; i++) {
        std::cin >> x[i] >> y[i];
    }
    double x0;
    std::cin >> x0;
    std::vector<std::vector<double>> dp(n, std::vector<double>(n));
    for (int i = 0; i < n; i++) {
        dp[i][0] = y[i];
    }
    for (int i = 1; i < n; i++) {
        for (int j = 1; j <= i; j++) {
            dp[i][j] = ((x0 - x[i - j]) * dp[i][j - 1] - (x0 - x[i]) * dp[i - 1][j - 1]) / (x[i] - x[i - j]);
        }
    }
    std::cout << dp[n - 1][n - 1] << std::endl;
}