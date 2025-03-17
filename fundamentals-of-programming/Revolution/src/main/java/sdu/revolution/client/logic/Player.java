package sdu.revolution.client.logic;

import com.spinyowl.legui.component.Label;
import com.spinyowl.legui.style.border.SimpleLineBorder;
import org.joml.Vector4f;
import sdu.revolution.client.Main;

import java.util.List;

import static com.spinyowl.legui.component.optional.align.HorizontalAlign.CENTER;
import static com.spinyowl.legui.component.optional.align.VerticalAlign.MIDDLE;
import static sdu.revolution.client.engine.gui.GuiLibrary.height;
import static sdu.revolution.client.engine.gui.GuiLibrary.width;

public class Player {
    public long balance;
    public boolean canExecute;
    public String name;
    public SoldierCardManager cardManager;
    public Player(String name) {
        this.balance = 0;
        this.name = name;
        canExecute = false;
        this.cardManager = new SoldierCardManager();
    }

    public void switchTurn() {
        canExecute = !canExecute;

        Main.menu.getGui().panels.get(4).clearChildComponents();
        if (canExecute) {
            Main.menu.getGui().panels.get(4).add(Main.menu.getGui().skipButton);
        } else {
            Main.menu.getGui().panels.get(4).add(Main.menu.getGui().mainLabels.get(4));
        }

        Label label = new Label(width / 2.0f - 200.0f, height / 2 - 50, 400, 100);
        label.getTextState().setText(canExecute ? "It's your turn." : "It's your opponent's turn.");
        label.getStyle().setTextColor(new Vector4f(1.0f, 1.0f, 1.0f, 0.0f));
        label.getStyle().setBorder(new SimpleLineBorder());
        label.getStyle().setFont("roboto-regular");
        label.getStyle().setFontSize(60f);
        label.getStyle().setHorizontalAlign(CENTER);
        label.getStyle().setVerticalAlign(MIDDLE);
        Main.menu.getGui().add(label);
        new Thread(() -> {
            try {
                for (int i = 1; i <= 100; ++i) {
                    label.getStyle().setTextColor(new Vector4f(1.0f, 1.0f, 1.0f, i / 100.0f));
                    Thread.sleep(1);
                }
                Thread.sleep(1000);
                for (int i = 1; i <= 100; ++i) {
                    label.getStyle().setTextColor(new Vector4f(1.0f, 1.0f, 1.0f, (100 - i) / 100.0f));
                    Thread.sleep(1);
                }
                Main.menu.getGui().remove(label);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }).start();
    }
}
