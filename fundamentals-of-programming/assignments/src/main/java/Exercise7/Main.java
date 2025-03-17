package Exercise7;

public class Main {
    public static void main(String[] args) {
        MyHashMap m = new MyHashMap();
        m.put("dog", "Bosco");
        m.put("dog", "Spot");
        m.put("cat", "Rags");
        System.out.println(m);
    }
}
