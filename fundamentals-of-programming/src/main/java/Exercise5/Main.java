package Exercise5;

import java.io.IOException;
import java.util.Scanner;

public class Main {
    public static StudentManager manager;

    public static void main(String[] args) throws IOException {
        manager = new StudentManager();
        Library.init();
        Scanner scanner = new Scanner(System.in);
        String path = scanner.nextLine();
        for (var i : StudentManager.FileTypes.values()) {
            for (int j = 1; j <= 3; ++j)
                manager.readFiles(path, j, i);
        }
        manager.printData();
        Library.exit();
    }
}
