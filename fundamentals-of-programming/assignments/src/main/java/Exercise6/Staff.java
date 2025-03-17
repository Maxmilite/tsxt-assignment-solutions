package Exercise6;

public class Staff extends Person {
    public int salarys;

    public Staff(String number, String name, int age, int salarys) {
        super(number, name, age);
        this.salarys = salarys;
    }

    @Override
    public double getCompareValue() {
        return this.salarys;
    }
}
