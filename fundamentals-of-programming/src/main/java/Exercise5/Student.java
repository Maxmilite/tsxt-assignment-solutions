package Exercise5;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Student {

    public long stuNum;
    public String stuName;
    public double creditTotal;
    public double creditScoreTotal;
    public List<String> curriculum;
    public Map<String, Integer> credit;

    public Student() {
        curriculum = new ArrayList<>();
        credit = new HashMap<>();
    }

    public Student(long stuNum, String stuName, int creditTotal, int creditScoreTotal) {
        this.stuNum = stuNum;
        this.stuName = stuName;
        this.creditTotal = creditTotal;
        this.creditScoreTotal = creditScoreTotal;
        curriculum = new ArrayList<>();
        credit = new HashMap<>();
    }

    public double getGPA() {
        if (creditTotal == 0) return 0.0;
        return this.creditScoreTotal / this.creditTotal;
    }

    public void addCurriculum(String x) {
        if (curriculum.contains(x)) return;
        curriculum.add(x);
    }

}
