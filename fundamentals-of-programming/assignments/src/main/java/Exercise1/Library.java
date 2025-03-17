package Exercise1;

public class Library {

    public static boolean isLeapYear(int x) {
        if (x % 100 == 0) {
            return x % 400 == 0;
        }
        return x % 4 == 0;
    }

    public static int getMonth(int year, int month) {
        int res = 0;
        switch (month) {
            case 1, 3, 5, 7, 8, 10, 12 -> res = 31;
            case 4, 6, 9, 11 -> res = 30;
            case 2 -> res = isLeapYear(year) ? 29 : 28;
        }
        return res;
    }

    public static int getWeek(int year, int month, int day) {
        if (month <= 2) {
            month += 12;
            year--;
        }
        int h = ((day + (26 * (month + 1) / 10) + (year % 100) + ((year % 100) / 4) + ((year / 100) / 4) + 5 * (year / 100)) % 7);
        if (h == 0) h = 7;
        return h;
    }

    public static String getString(int month) {
        switch (month) {
            case 1 -> {
                return "Jan";
            }
            case 2 -> {
                return "Feb";
            }
            case 3 -> {
                return "Mar";
            }
            case 4 -> {
                return "Apr";
            }
            case 5 -> {
                return "May";
            }
            case 6 -> {
                return "Jun";
            }
            case 7 -> {
                return "Jul";
            }
            case 8 -> {
                return "Aug";
            }
            case 9 -> {
                return "Sep";
            }
            case 10 -> {
                return "Oct";
            }
            case 11 -> {
                return "Nov";
            }
            case 12 -> {
                return "Dec";
            }
        }
        return null;
    }
}
