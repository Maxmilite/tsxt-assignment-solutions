package sdu.revolution.client.engine.gui.models;

import com.spinyowl.legui.component.ImageView;
import com.spinyowl.legui.component.Label;
import com.spinyowl.legui.component.Panel;
import com.spinyowl.legui.component.optional.align.HorizontalAlign;
import com.spinyowl.legui.event.MouseClickEvent;
import com.spinyowl.legui.listener.MouseClickEventListener;
import sdu.revolution.client.Main;
import sdu.revolution.client.engine.gui.GuiLibrary;
import sdu.revolution.client.logic.SoldierCardManager;

public class Card extends Panel {

    public String name, iconPath;
    public int index;
    public ImageView icon;

    public Card(String name, String iconPath) {
        super();
        this.name = name;
        this.iconPath = iconPath;
        this.index = SoldierCardManager.getNextIndex();
        this.init();
    }

    private void call() {
        Main.Logger.info(this, "Card " + index + " with name " + name + " was called.");
    }

    public void init() {
        // 64 * 64
        this.setSize(70, 85);
        icon = GuiLibrary.createCardIcon(iconPath);
        Label nameLabel = new Label(name, 4, 69, 64, 16);
        nameLabel.getStyle().setFont("YaHei Mono");
        nameLabel.getStyle().setFontSize(16f);
        nameLabel.getStyle().setHorizontalAlign(HorizontalAlign.CENTER);
        this.add(icon);
        this.add(nameLabel);
        this.getListenerMap().addListener(MouseClickEvent.class, (MouseClickEventListener) event -> {
            if (event.getAction() == MouseClickEvent.MouseClickAction.RELEASE) {
                call();
            }
        });
        nameLabel.getListenerMap().addListener(MouseClickEvent.class, (MouseClickEventListener) event -> {
            if (event.getAction() == MouseClickEvent.MouseClickAction.RELEASE) {
                call();
            }
        });
        icon.getListenerMap().addListener(MouseClickEvent.class, (MouseClickEventListener) event -> {
            if (event.getAction() == MouseClickEvent.MouseClickAction.RELEASE) {
                call();
            }
        });
    }
}
