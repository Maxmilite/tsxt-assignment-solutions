package sdu.revolution.client.logic;

public class PlayerEntity {
    public int cnt = 0, frt[] = new int[100005], end[] = new int[100005], num, money;
    public int L[] = new int[100005], R[] = new int[100005];
    public boolean tec[] = new boolean[201];

    public PlayerEntity(int ty, int hav) {
        num = ty;
        money = hav;
    }

    public Army a[] = new Army[100005];

    public void create(Army now, int x) {
        if (frt[x] == 0) frt[x] = cnt;
        a[cnt] = now;
        R[end[x]] = cnt;
        L[cnt] = end[x];
        end[x] = cnt;
    }

    public void delete(int now, int x) {
        if (now == frt[x]) {
            frt[x] = R[now];
            L[R[now]] = 0;
        } else if (now == end[x]) {
            end[x] = L[now];
            R[L[now]] = 0;
        } else {
            L[R[now]] = R[now];
            R[L[now]] = L[now];
        }
        L[now] = R[now] = 0;
    }

    public void add(int num, int x) {
        if (frt[x] == 0) frt[x] = num;
        R[end[x]] = num;
        L[num] = end[x];
        end[x] = num;
    }

}
