package Exercise1;

import java.util.Scanner;

public class Main {
    public static StringBuilder[] builders = new StringBuilder[45];
    public static final String TITLE = "Sun\tMon\tTue\tWed\tThu\tFri\tSat\t";

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int year = scanner.nextInt();
        Calendar[] calendar = new Calendar[15];
        for (int i = 1; i <= 44; ++i)
            builders[i] = new StringBuilder();
        for (int i = 1; i <= 12; ++i) {
            calendar[i] = new Calendar();
            calendar[i].init(year, i);
        }
        for (int i = 1, cur = 1; i <= 12; ++i) {
            builders[cur].append("\t\t\t").append(Library.getString(i)).append("\t\t\t\t\t");
            builders[cur + 1].append(TITLE).append("\t");
            for (int k = 1; k <= 6; ++k) {
                for (int l = 1; l <= 7; ++l)
                    builders[cur + k + 1].append(calendar[i].matrix[k][l] == 0 ? " " : calendar[i].matrix[k][l]).append("\t");
                builders[cur + k + 1].append("\t");
            }
            if (i % 4 == 0) cur += 8;
        }
        for (int i = 1; i <= 24; ++i)
            System.out.println(builders[i]);
    }
}
