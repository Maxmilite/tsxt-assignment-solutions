package sdu.revolution.client.logic;

public class Army {
    public int HP, ATK, Range, Speed, Cost, Type, Place, Num;

    public Army(int x, int y, int now, int arv) {
        Type = Data.Type_ar[x][y];
        Cost = Data.Cost_ar[Type];
        HP = Data.HP_re[Type];
        ATK = Data.ATK_re[Type];
        Range = Data.Range_re[Type];
        Speed = Data.Speed_re[Type];
        Place = arv;
        Num = now;
    }

    public int getCost() {
        return Cost;
    }

    public int getType() {
        return Type;
    }

    public int getNum() {
        return Num;
    }

    public int getATK() {
        return ATK;
    }

    public int getHP() {
        return HP;
    }

    public int getSpeed() {
        return Speed;
    }

    public int getRange() {
        return Range;
    }

    public int getPlace() {
        return Place;
    }

    public void change(int now) {
        Place = now;
        return;
    }

    public void suffer(int ing) {
        HP -= ing;
        return;
    }

    public int sale() {
        HP = 0;
        return Cost / 2;
    }

    public void sus() {//悬浮
        //展示属性
        //加载UI菜单
    }

    public int click() {//点击
        int choose = -1, read;//选择操作
        if (choose == 0) {
            read = -1;//选择变更的地点
            change(read);
            return 0;
        }
        if (choose == 1) {
            return sale();
        }
        return 0;
    }


}
