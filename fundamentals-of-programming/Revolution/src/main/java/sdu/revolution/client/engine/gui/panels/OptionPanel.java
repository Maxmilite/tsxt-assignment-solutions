package sdu.revolution.client.engine.gui.panels;

import com.spinyowl.legui.component.Button;
import com.spinyowl.legui.component.Label;
import com.spinyowl.legui.component.optional.align.HorizontalAlign;
import com.spinyowl.legui.component.optional.align.VerticalAlign;
import com.spinyowl.legui.event.MouseClickEvent;
import com.spinyowl.legui.listener.MouseClickEventListener;
import com.spinyowl.legui.style.color.ColorUtil;
import com.spinyowl.legui.style.length.Length;
import com.spinyowl.legui.style.length.LengthType;
import org.joml.Vector2f;
import org.lwjgl.glfw.GLFW;
import sdu.revolution.client.Main;
import sdu.revolution.client.engine.gui.GuiLibrary;
import sdu.revolution.client.engine.gui.PanelInstance;

import java.util.Arrays;
import java.util.List;

public class OptionPanel extends PanelInstance {

    @SuppressWarnings("rawtypes,unchecked")
    @Override
    public void init() {
        GuiLibrary.setPanelStyle(this);
        this.getStyle().setVerticalAlign(VerticalAlign.MIDDLE);
        this.getStyle().setHorizontalAlign(HorizontalAlign.CENTER);
//        this.setSize(400, 600);
        this.setSize(400, 0);
//        this.setPosition((width / 2) - 200, (height / 2) - 300);
        this.setPosition((width / 2) - 200, (height / 2));
        Label title = new Label("Game Options");
        title.getStyle().setTextColor(ColorUtil.rgba(0, 255, 255, 1f));
        title.getStyle().setFontSize(36f);
        title.getStyle().setFont("Impact");
        title.getStyle().setVerticalAlign(VerticalAlign.TOP);
        title.getStyle().setHorizontalAlign(HorizontalAlign.CENTER);
        title.getStyle().setPaddingTop(new Length(20f, LengthType.PIXEL));
        title.setSize(this.getSize());
        this.add(title);
        float panelWidth = this.getSize().x;
        Button close = new Button("\uE14C", panelWidth - 60, 18, 40, 40);
        GuiLibrary.setCloseButtonStyle(close, 36f);
        close.getListenerMap().addListener(MouseClickEvent.class, (MouseClickEventListener) event -> {
            if (event.getAction() == MouseClickEvent.MouseClickAction.RELEASE) {
                Main.menu.getGui().close(this, true);
            }
        });
        this.add(close);
        List<Button> buttons = Arrays.asList(
                new Button("Settings", panelWidth / 2 - 90f, 10 + 1 * 80f, 180f, 60f),
                new Button("Info", panelWidth / 2 - 90f, 10 + 2 * 80f, 180f, 60f),
                new Button("Map", panelWidth / 2 - 90f, 10 + 3 * 80f, 180f, 60f),
                new Button("Players", panelWidth / 2 - 90f, 10 + 4 * 80f, 180f, 60f),
                new Button("Leave Server", panelWidth / 2 - 90f, 10 + 5 * 80f, 180f, 60f),
                new Button("Quit to Desktop", panelWidth / 2 - 90f, 10 + 6 * 80f, 180f, 60f)
        );
        buttons.get(5).getListenerMap().addListener(MouseClickEvent.class, (MouseClickEventListener) event -> {
            if (event.getAction() == MouseClickEvent.MouseClickAction.RELEASE) {
                GuiLibrary.prompt("Are you sure to quit to desktop?",
                        () -> GLFW.glfwSetWindowShouldClose(Main.getEngine().getWindow().getHandle(), true),
                        () -> {
                        });
//                GLFW.glfwSetWindowShouldClose(Main.getEngine().getWindow().getHandle(), true);
            }
        });
        buttons.forEach(GuiLibrary::setButtonStyle);
        buttons.forEach(this::add);
    }

    @Override
    public void close() {
        if (Main.menu.getGui().contains(this)) {
            GuiLibrary.runSlide(this, 200, new Vector2f(400, 0), new Vector2f((width / 2) - 200, (height / 2)));
            this.isOpen = false;
            new Thread(() -> {
                try {
                    Thread.sleep(250);
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }
                Main.menu.getGui().remove(this);
            }).start();
        }
    }

    @Override
    public void call() {
        if (!Main.menu.getGui().contains(this)) {
            Main.menu.getGui().add(this);
            GuiLibrary.runSlide(this, 200, new Vector2f(400, 600), new Vector2f((width / 2) - 200, (height / 2) - 300));
            new Thread(() -> {
                try {
                    Thread.sleep(250);
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }
                this.isOpen = true;
            }).start();
        }
    }
}
