package Exercise6;

public class Student extends Person {
    public int english, math, program;

    public Student(String number, String name, int age, int english, int math, int program) {
        super(number, name, age);
        this.english = english;
        this.math = math;
        this.program = program;
    }

    public double getGPA() {
        return (english * 6.0 / 15 + math * 5.0 / 15 + program * 4.0 / 15) / 10 - 5;
    }

    @Override
    public double getCompareValue() {
        return this.getGPA();
    }
}
