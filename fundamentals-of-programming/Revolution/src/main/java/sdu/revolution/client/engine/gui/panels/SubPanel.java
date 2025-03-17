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
import sdu.revolution.client.Main;
import sdu.revolution.client.engine.gui.GuiLibrary;
import sdu.revolution.client.engine.gui.PanelInstance;

import java.util.Arrays;
import java.util.List;

public class SubPanel extends PanelInstance {
    public String title;
    public SubPanel(String title) {
        this.isWindow = true;
        this.isOpen = false;
        this.width = GuiLibrary.width;
        this.height = GuiLibrary.height;
        this.title = title;
        init();
    }
    @SuppressWarnings("rawtypes, unchecked")
    @Override
    public void init() {
        GuiLibrary.setPanelStyle(this);
        this.setPosition(40, 40);
        this.setSize(width - 80f, height - 80f);
        this.getStyle().setVerticalAlign(VerticalAlign.TOP);
        this.getStyle().setHorizontalAlign(HorizontalAlign.CENTER);
        float panelWidth = this.getSize().x;
        float panelHeight = this.getSize().y;
        Label titleLabel = new Label(title);
        titleLabel.getStyle().setTextColor(ColorUtil.rgba(0, 255, 255, 1f));
        titleLabel.getStyle().setFontSize(48f);
        titleLabel.getStyle().setFont("Impact");
        titleLabel.getStyle().setVerticalAlign(VerticalAlign.TOP);
        titleLabel.getStyle().setHorizontalAlign(HorizontalAlign.CENTER);
        titleLabel.getStyle().setPaddingTop(new Length(20f, LengthType.PIXEL));
        titleLabel.setSize(this.getSize());
        this.add(titleLabel);
        List<Button> buttons = Arrays.asList(
                new Button(panelWidth / 2 - 640f, panelHeight - 100f, 180f, 60f),
                new Button(panelWidth / 2 - 420f, panelHeight - 100f, 180f, 60f),
                new Button(panelWidth / 2 - 200f, panelHeight - 100f, 180f, 60f),
                new Button(panelWidth / 2 + 20f, panelHeight - 100f, 180f, 60f),
                new Button(panelWidth / 2 + 240f, panelHeight - 100f, 180f, 60f),
                new Button(panelWidth / 2 + 460f, panelHeight - 100f, 180f, 60f)
        );
        buttons.forEach(GuiLibrary::setButtonStyle);
        buttons.forEach(this::add);
        Button close = new Button("\uE14C", panelWidth - 60, 20, 40, 40);
        GuiLibrary.setCloseButtonStyle(close, 60f);
        close.getListenerMap().addListener(MouseClickEvent.class, (MouseClickEventListener) event -> {
            if (event.getAction() == MouseClickEvent.MouseClickAction.RELEASE) {
                Main.menu.getGui().close(this, true);
            }
        });
        this.add(close);
    }

    @Override
    public void call() {
        Main.menu.getGui().add(this);
        GuiLibrary.runTransition(this, 400, 0, 0, GuiLibrary.TRANSITION_BACKGROUND | GuiLibrary.TRANSITION_FONT);
    }

    @Override
    public void close() {
        GuiLibrary.runTransition(this, 0, 0, 400,
                GuiLibrary.TRANSITION_BACKGROUND | GuiLibrary.TRANSITION_FONT | GuiLibrary.TRANSITION_REMOVAL);
        new Thread(() -> {
            try {
                Thread.sleep(450);
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
            Main.menu.getGui().remove(this);
        }).start();
    }
}
