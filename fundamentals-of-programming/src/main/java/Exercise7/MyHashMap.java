package Exercise7;

import java.util.Arrays;
import java.util.Comparator;
import java.util.List;
import java.util.Objects;

public class MyHashMap {
    public final static int DEFAULT_CAPACITY = 16;
    public final static float DEFAULT_LOAD_FACTOR = 0.75f;

    public int capacity;
    public float loadFactor;
    public int size = 0;
    public int time_point = 0;

    public MyNode<String>[] table;

    public MyHashMap() {
        this(DEFAULT_CAPACITY, DEFAULT_LOAD_FACTOR);
    }

    public int calc(int x) {
        int res = 1;
        while (res <= x) res <<= 1;
        return res;
    }

    @SuppressWarnings("unchecked")
    public MyHashMap(int capacity, float loadFactor) {
        this.capacity = calc(capacity);
        this.loadFactor = loadFactor;
        this.table = new MyNode[capacity];
    }

    public void put(String key, String value) {
        int index = key.hashCode() % table.length;
        MyNode<String> current = table[index];
        if (current != null) {
            while (current.next != null) {
                if (current.key.equals(key)) {
                    current.value = value;
                    current.time_point = time_point++;
                    return;
                }
                current = current.next;
            }
            table[index] = new MyNode<>(key, value, current, null, time_point++);
            current.next = table[index];
        } else {
            table[index] = new MyNode<>(key, value, null, null, time_point++);
        }
        size++;
    }

    public String get(String key) {
        int index = key.hashCode() % table.length;
        MyNode<String> current = table[index];
        while (current != null) {
            if (current.key.equals(key)) {
                return current.value;
            }
            current = current.next;
        }
        return null;
    }

    @Override
    public String toString() {
        StringBuilder s = new StringBuilder();
        s.append("[");
        List<MyNode<String>> list = new java.util.ArrayList<>(Arrays.stream(table).filter(Objects::nonNull).toList());
        list.sort(Comparator.comparingInt(x -> x.time_point));
        for (MyNode<String> stringMyNode : list) {
            s.append(stringMyNode.key).append(":").append(stringMyNode.value).append(",");
        }
        s.deleteCharAt(s.length() - 1).append("]");
        return s.toString();
    }
}
