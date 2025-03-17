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
    double x0;
    std::cin >> x0;

    double y = a[n], z = a[n];
    for (int j = n - 1; j > 0; j--) {
        y = a[j] + x0 * y;
        z = y + x0 * z;
    }
    y = a[0] + x0 * y;

    std::cout << y << ' ' << z << std::endl;

    return 0;
}