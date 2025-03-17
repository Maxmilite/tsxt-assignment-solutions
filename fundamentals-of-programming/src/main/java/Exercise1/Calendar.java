package Exercise1;

public class Calendar {
    public int[][] matrix;

    public Calendar() {
        matrix = new int[10][10];
    }

    public void init(int year, int month) {
        int curLine = 1, months = Library.getMonth(year, month);
        for (int i = 1; i <= months; ++i) {
            int cur = Library.getWeek(year, month, i);
            matrix[curLine][cur] = i;
            if (cur == 7) ++curLine;
        }
    }
}
