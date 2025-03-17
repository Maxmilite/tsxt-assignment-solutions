package sdu.revolution.client.logic;

public class Area {
    public int cnt = 0, frt = 0, end = 0, num, cost, blong = -1;
    public int L[] = new int[100005], R[] = new int[100005];
    public Building a[] = new Building[100005];

    public Area(int now, int Cost) {
        num = now;
        cost = Cost;
    }

    public void Creat(int x, int y) {
        cnt++;
        if (frt == 0) frt = cnt;
        a[cnt] = new Building(x, y, cnt);
        R[end] = cnt;
        L[cnt] = end;
        end = cnt;
        return;
    }

    public void Delet(int now) {
        if (now == frt) {
            frt = R[now];
            L[R[now]] = 0;
        } else if (now == end) {
            end = L[now];
            R[L[now]] = 0;
        } else {
            L[R[now]] = R[now];
            R[L[now]] = L[now];
        }
        L[now] = R[now] = 0;
        return;
    }

    public int ch_bl(int now) {
        blong = now;
        return cost;
    }


    public void sus() {//悬浮
        //展示属性
        //展开Buildings
    }

    public int click() {//点击
        int choose = -1, read;//选择操作
        if (choose == 0) {
            //进入对应Buildings
            read = -1;
            return read;
        }
        if (choose == 1) {
            read = -1;
            return ch_bl(read);
        }
        if (choose == 2) {
            //进入对应建筑商店
        }
        return 0;
    }
}
