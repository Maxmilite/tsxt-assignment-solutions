package sdu.revolution.client.engine.gui;

import com.spinyowl.legui.component.Panel;
import sdu.revolution.client.Main;
import sdu.revolution.client.engine.gui.models.Card;

import java.util.List;

public class CardStack extends Panel {

    public CardStack() {
        super(0, 0, GuiLibrary.width - 200, 100);
        init();
    }

    public void init() {
        update();
    }

    public void update() {
        this.clearChildComponents();
        List<Card> cardList = Main.logic.player.cardManager.cardList;
        for (int i = 0; i < cardList.size(); ++i) {
            Card card = cardList.get(i);
            card.setPosition(85 * (i) + 15f, 7.5f);
        }
        cardList.forEach(this::add);
    }
}
