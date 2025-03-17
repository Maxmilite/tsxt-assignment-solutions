package Exercise6;

import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.stream.Collectors;

public class Manager {
    public List<Person> list;

    public Manager() {
        list = new ArrayList<>();
    }

    public void readData(InputStream stream) {
        Scanner scanner = new Scanner(stream);
        while (scanner.hasNextLine()) {
            String[] s = scanner.nextLine().split(" ");
            if (s.length == 4) {
                list.add(new Staff(s[0], s[1], Integer.parseInt(s[2]), Integer.parseInt(s[3])));
            } else if (s.length == 6) {
                list.add(new Student(s[0], s[1], Integer.parseInt(s[2]), Integer.parseInt(s[3]), Integer.parseInt(s[4]), Integer.parseInt(s[5])));
            }
        }
    }

    public void printByAgeAndNumber() {
        List<Person> var1 = new ArrayList<>(list);
        var1.sort((x, y) -> {
            if (x.age == y.age) {
                return x.number.compareTo(y.number);
            }
            return x.age - y.age;
        });
        var1.forEach((i) -> {
            System.out.print("Number:" + i.number + " Name: " + i.name + " Age: " + i.age + " ");
            if (i instanceof Staff) System.out.println("Salary: " + ((Staff) i).salarys);
            else if (i instanceof Student) System.out.println("GPA: " + String.format("%.2f", ((Student) i).getGPA()));
            else System.out.println();
        });
    }

    public void printByOrder() {
        List<Person> var1 = new ArrayList<>(list).stream().filter(i -> i instanceof Staff).collect(Collectors.toList());
        List<Person> var2 = new ArrayList<>(list).stream().filter(i -> i instanceof Student).collect(Collectors.toList());
        var1.sort((x, y) -> (int) (y.getCompareValue() - x.getCompareValue()));
        var2.sort((x, y) -> ((y.getCompareValue() - x.getCompareValue()) == 0 ? 0 : (y.getCompareValue() - x.getCompareValue() < 0 ? -1 : 1)));
        var1.forEach((i) -> {
            System.out.print("Number:" + i.number + " Name: " + i.name + " Age: " + i.age + " ");
            System.out.println("Salary: " + ((Staff) i).salarys);
        });
        System.out.println();
        var2.forEach((i) -> {
            System.out.print("Number:" + i.number + " Name: " + i.name + " Age: " + i.age + " ");
            System.out.println("GPA: " + String.format("%.2f", ((Student) i).getGPA()));
        });
    }
}
