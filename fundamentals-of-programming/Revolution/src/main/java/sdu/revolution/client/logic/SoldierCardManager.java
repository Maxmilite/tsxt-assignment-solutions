package sdu.revolution.client.logic;

import sdu.revolution.client.engine.gui.models.Card;

import java.util.ArrayList;
import java.util.List;

public class SoldierCardManager {
    public List<Card> cardList;
    public static int pool;

    public SoldierCardManager() {
        pool = 0;
        cardList = new ArrayList<>();
        for (int i = 1; i <= 10; ++i)
            cardList.add(new Card("Soldier", "resources/textures/soldier.jpg"));
    }

    public static int getNextIndex() {
        return ++pool;
    }
}
