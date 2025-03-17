package Exercise5;

import com.alibaba.fastjson2.JSONObject;

import java.io.*;
import java.util.*;

public class StudentManager {
    public enum FileTypes {
        STUDENT, CURRICULUM, SCORE
    }

    public Map<Long, Student> studentMap;

    public static class Curriculum {
        public String name;
        public int credit;

        public Curriculum(String name, int credit) {
            this.name = name;
            this.credit = credit;
        }
    }

    public Map<String, Curriculum> lessonMap;

    public StudentManager() {
        studentMap = new HashMap<>();
        lessonMap = new HashMap<>();
    }

    public void readFile(String path, FileTypes types) {
        try {
            Scanner scanner = new Scanner(new FileInputStream(path));
            while (scanner.hasNext()) {
                switch (types) {
                    case STUDENT -> {
                        long var2 = scanner.nextLong();
                        String var1 = scanner.next();
                        if (studentMap.containsKey(var2)) {
                            System.err.printf((Library.STUDENT_REWRITTEN) + "%n", var2);
                        }
                        studentMap.put(var2, new Student(var2, var1, 0, 0));
                    }
                    case CURRICULUM -> {
                        String var1 = scanner.next(), var2 = scanner.next();
                        int var3 = scanner.nextInt();
                        if (lessonMap.containsKey(var1)) {
                            System.err.printf((Library.LESSON_REWRITTEN) + "%n", var1, var2);
                        }
                        lessonMap.put(var1, new Curriculum(var2, var3));
                    }
                    case SCORE -> {
                        long var1 = scanner.nextLong();
                        if (!studentMap.containsKey(var1)) {
                            System.err.printf((Library.STUDENT_NOT_FOUND) + "%n", var1);
                            scanner.next();
                            scanner.nextInt();
                            continue;
                        }
                        String var2 = scanner.next();
                        int var3 = scanner.nextInt();
                        if (!lessonMap.containsKey(var2)) {
                            System.err.printf((Library.CURRICULUM_NOT_FOUND) + "%n", var2);
                            continue;
                        }
                        try {
                            if (studentMap.get(var1).credit.containsKey(var2)) {
                                studentMap.get(var1).creditScoreTotal -= (studentMap.get(var1).credit.get(var2) / 10.0 - 5) * lessonMap.get(var2).credit;
                                studentMap.get(var1).creditTotal -= lessonMap.get(var2).credit;
                                System.err.printf((Library.CREDIT_REWRITTEN) + "%n", var1, var2, var3);
                            }
                            studentMap.get(var1).credit.put(var2, var3);
                            studentMap.get(var1).creditScoreTotal += (var3 / 10.0 - 5) * lessonMap.get(var2).credit;
                            studentMap.get(var1).creditTotal += lessonMap.get(var2).credit;
                        } catch (NullPointerException e) {
                            System.err.printf((Library.CURRICULUM_NOT_FOUND) + "%n", var2);
                        }
                    }
                }
            }
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        }
    }

    public List<Student> getEntries() {
        return new ArrayList<>(studentMap.values());
    }

    public void readFiles(String path, int x) {
        this.readFiles(path, x, FileTypes.STUDENT);
        this.readFiles(path, x, FileTypes.CURRICULUM);
        this.readFiles(path, x, FileTypes.SCORE);
    }

    public void readFiles(String path, int x, FileTypes type) {
        switch (type) {
            case STUDENT -> this.readFile(path + String.format("S%d.txt", x), FileTypes.STUDENT);
            case CURRICULUM -> this.readFile(path + String.format("C%d.txt", x), FileTypes.CURRICULUM);
            case SCORE -> this.readFile(path + String.format("A%d.txt", x), FileTypes.SCORE);
        }
    }

    public void printData() {
        List<Student> list = this.getEntries();
        list.sort((x, y) -> {
            double var1 = y.getGPA() - x.getGPA();
            if (var1 == 0) return 0;
            if (var1 < 0) return -1;
            return 1;
        });
        list.forEach((i) -> System.out.println(i.stuNum + " " + i.stuName + " " + String.format("%.2f", i.getGPA())));
    }
}
