package sdu.revolution.client.engine.gui.panels;

import com.spinyowl.legui.component.Button;
import com.spinyowl.legui.component.Label;
import com.spinyowl.legui.component.optional.align.HorizontalAlign;
import com.spinyowl.legui.component.optional.align.VerticalAlign;
import com.spinyowl.legui.event.MouseClickEvent;
import com.spinyowl.legui.listener.MouseClickEventListener;
import com.spinyowl.legui.style.length.Length;
import com.spinyowl.legui.style.length.LengthType;
import org.joml.Vector2f;
import sdu.revolution.client.Main;
import sdu.revolution.client.engine.gui.GuiLibrary;
import sdu.revolution.client.engine.gui.PanelInstance;

public class PromptPanel extends PanelInstance {
    public PromptPanel(String content, Runnable yesFunction, Runnable noFunction) {
        GuiLibrary.setAlign(this, VerticalAlign.MIDDLE, HorizontalAlign.CENTER);
        GuiLibrary.setPanelStyle(this);
        this.setSize(400, 200);
        this.setPosition((width / 2) - 200, (height / 2) - 100);
        Label title = new Label("Prompt");
        title.getStyle().setTextColor(GuiLibrary.getColor(GuiLibrary.Color.AQUA));
        GuiLibrary.setFont(title, "Impact", 32f);
        GuiLibrary.setAlign(title, VerticalAlign.TOP, HorizontalAlign.CENTER);
        title.getStyle().setPaddingTop(new Length(20f, LengthType.PIXEL));
        title.setSize(this.getSize());
        this.add(title);
        Label val = new Label(content);
        val.getStyle().setTextColor(GuiLibrary.getColor(GuiLibrary.Color.AQUA));
        val.getStyle().setFontSize(24f);
        val.getStyle().setFont("YaHei");
        GuiLibrary.setAlign(val, VerticalAlign.TOP, HorizontalAlign.CENTER);
        val.getStyle().setPaddingTop(new Length(72f, LengthType.PIXEL));
        val.setSize(this.getSize());
        this.add(val);
        float panelWidth = this.getSize().x;
        float panelHeight = this.getSize().y;
        Button close = new Button("\uE14C", panelWidth - 48, 18, 30, 30);
        GuiLibrary.setCloseButtonStyle(close, 24f);
        close.getListenerMap().addListener(MouseClickEvent.class, (MouseClickEventListener) event -> {
            if (event.getAction() == MouseClickEvent.MouseClickAction.RELEASE) {
                Main.menu.getGui().close(this, true);
            }
        });
        this.add(close);
        Button yesButton = new Button("Yes", panelWidth / 2 - 150f, panelHeight - 70f, 120f, 50f), noButton = new Button("No", panelWidth / 2 + 30f, panelHeight - 70f, 120f, 50f);
        GuiLibrary.setButtonStyle(yesButton);
        GuiLibrary.setButtonStyle(noButton);
        yesButton.getListenerMap().addListener(MouseClickEvent.class, (MouseClickEventListener) event -> {
            if (event.getAction() == MouseClickEvent.MouseClickAction.RELEASE) {
                yesFunction.run();
                Main.menu.getGui().close(this, true);
            }
        });
        noButton.getListenerMap().addListener(MouseClickEvent.class, (MouseClickEventListener) event -> {
            if (event.getAction() == MouseClickEvent.MouseClickAction.RELEASE) {
                noFunction.run();
                Main.menu.getGui().close(this, true);
            }
        });
        this.add(yesButton);
        this.add(noButton);
        this.setSize(new Vector2f(400, 0));
        this.setPosition(new Vector2f((width / 2) - 200, (height / 2)));
    }

    @Override
    public void close() {
        GuiLibrary.runSlide(this, 100, new Vector2f(400, 0), new Vector2f((width / 2) - 200, (height / 2)));
        new Thread(() -> {
            try {
                Thread.sleep(250);
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
            Main.menu.getGui().remove(this);
        }).start();
    }

    @Override
    public void call() {
        Main.menu.getGui().add(this);
        GuiLibrary.runSlide(this, 100, new Vector2f(400, 200), new Vector2f((width / 2) - 200, (height / 2) - 100));
    }
}
