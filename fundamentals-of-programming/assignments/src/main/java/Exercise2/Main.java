package Exercise2;

import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int x = scanner.nextInt();
        StringBuilder ans = new StringBuilder();
        int var1 = x;
        while (var1 > 0) {
            ans.append(var1 & 1);
            var1 /= 2;
        }
        ans.reverse();
        ans.append("#");
        for (int i = 2; i <= x && x != 1; ++i) {
            while (x % i == 0) {
                x /= i;
                ans.append(i).append(",");
            }
        }
        ans.deleteCharAt(ans.length() - 1);
        System.out.println(ans);
    }
}
