package Exercise6;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class Main {
    public static Manager manager;

    public static void main(String[] args) throws FileNotFoundException {
        manager = new Manager();
        String path = new Scanner(System.in).nextLine();
        Scanner scanner = new Scanner(new FileInputStream(path));
        while (scanner.hasNextLine()) {
            Reader.readLine(scanner);
        }
        manager.printByAgeAndNumber();
        System.out.println();
        manager.printByOrder();
    }
}
