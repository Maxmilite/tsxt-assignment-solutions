package Exercise7;

public class MyNode<T> {
    public MyNode<T> prev, next;
    public T key, value;
    public int time_point;

    public MyNode(T key, T value, MyNode<T> prev, MyNode<T> next, int time_point) {
        this.prev = prev;
        this.next = next;
        this.value = value;
        this.key = key;
        this.time_point = time_point;
    }

    public MyNode<T> getPrev() {
        return prev;
    }

    public T getKey() {
        return key;
    }

    public void setKey(T key) {
        this.key = key;
    }

    public MyNode<T> getNext() {
        return next;
    }

    public void setValue(T value) {
        this.value = value;
    }

    public T getValue() {
        return value;
    }

    public void setPrev(MyNode<T> prev) {
        this.prev = prev;
    }

    public void setNext(MyNode<T> next) {
        this.next = next;
    }
}
