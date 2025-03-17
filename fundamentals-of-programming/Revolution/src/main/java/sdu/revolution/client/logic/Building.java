package sdu.revolution.client.logic;

public class Building {
    public int Blood, Type, Cost, Num;

    public Building(int x, int y, int now) {
        Type = Data.Type_bl[x][y];
        Blood = Data.Blood_re[Type];
        Cost = Data.Cost_bl[Type];
        Num = now;
    }

    public void suffer(int x) {
        Blood -= x;
        return;
    }

    public int getBlood() {
        return Blood;
    }

    public int sale() {
        Blood = 0;
        return Cost / 2;
    }

    public int getNum() {
        return Num;
    }

    public int getType() {
        return Type;
    }

    public int getCost() {
        return Cost;
    }

    public int repair() {
        int ls = Blood / Data.Blood_re[Type] * Cost;
        Blood = Data.Blood_re[Type];
        return ls;
    }

    public void sus() {//悬浮
        //展示属性
        //加载UI菜单
    }

    public int click() {//点击
        int choose = -1, read;//选择操作
        if (choose == 0) {
            return -1 * repair();
        }
        if (choose == 1) {
            return sale();
            //待完善：区域链表修改
        }
        if (choose == 2) {
            //进入商店
        }
        return 0;
    }
}
