package Exercise6;

public abstract class Person {
    public String number;
    public String name;
    public int age;

    public Person(String number, String name, int age) {
        this.number = number;
        this.name = name;
        this.age = age;
    }

    public abstract double getCompareValue();
}
