package sdu.revolution.client.engine.gui;

import com.spinyowl.legui.component.Button;
import com.spinyowl.legui.component.Label;
import com.spinyowl.legui.component.Panel;
import com.spinyowl.legui.component.optional.TextState;
import com.spinyowl.legui.component.optional.align.HorizontalAlign;
import com.spinyowl.legui.component.optional.align.VerticalAlign;
import com.spinyowl.legui.event.MouseClickEvent;
import com.spinyowl.legui.listener.MouseClickEventListener;
import com.spinyowl.legui.style.Style;
import com.spinyowl.legui.style.border.SimpleLineBorder;
import com.spinyowl.legui.style.color.ColorConstants;
import com.spinyowl.legui.style.color.ColorUtil;
import com.spinyowl.legui.style.font.FontRegistry;
import org.joml.Vector4f;
import sdu.revolution.client.Main;
import sdu.revolution.client.engine.gui.info.InfoLibrary;
import sdu.revolution.client.engine.gui.panels.DebugPanel;
import sdu.revolution.client.engine.gui.panels.SubPanel;
import sdu.revolution.client.engine.gui.panels.OptionPanel;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;


// Todo: Reduce duplicate code
// Finished on 2023-01-12 22:33

public class GUI extends Panel {
    public int width, height;
    private Style panelStyle;
    public SubPanel subPanel;
    public OptionPanel optionPanel;
    public DebugPanel debugPanel;
    public CardStack cards;
    public boolean logon;
    public final List<PanelInstance> panelStack;
    public boolean lock;

    public void call(PanelInstance instance, boolean inStack) {
        instance.call();
        if (inStack)
            panelStack.add(instance);
    }

    public void close(PanelInstance instance, boolean inStack) {
        instance.close();
        if (inStack)
            panelStack.remove(instance);
    }

    private void createStyle() {
        panelStyle = new Style();
        panelStyle.getBackground().setColor(new Vector4f(0.0f, 0.0f, 0.0f, 0.75f));
        panelStyle.setBorder(new SimpleLineBorder(new Vector4f(1f, 1f, 1f, 1f), 1));
    }
    public List<Label> mainLabels;
    public List<Panel> panels;
    public Button skipButton;

    private void createUtilityPanels() {
        // Top Left Panel
        // Top Panel
        // Top Right (Config) Panel
        // Bottom Panel
        // Phase Panel
        mainLabels = Arrays.asList(
                new Label(InfoLibrary.getTurnStatus()),
                new Label(InfoLibrary.getGUIStatus()),
                new Label("Config"),
                new Label(""),
                new Label("Not Your Turn")
        );
        panels = Arrays.asList(new Panel(0, 0, 128, 128), // Top Left Panel
                new Panel(128, 0, width - 328, 64), // Top Panel
                new Panel(width - 200, 0, 200, 64), // Top Right (Config) Panel
                new Panel(0, height - 100, width - 200, 100), // Bottom Panel
                new Panel(width - 200, height - 100, 200, 100) // Phase Panel
        );
        mainLabels.forEach((e) -> {
            e.getStyle().setTextColor(ColorUtil.rgba(0, 255, 255, 1f));
            e.getStyle().setHorizontalAlign(HorizontalAlign.CENTER);
            e.getStyle().setVerticalAlign(VerticalAlign.MIDDLE);
            e.getStyle().setFontSize(24f);
            e.getStyle().setFont("roboto-regular");
        });
        panels.forEach((e) -> e.setStyle(panelStyle));
        for (int i = 0; i < 5; i++) {
            mainLabels.get(i).setSize(panels.get(i).getSize());
            panels.get(i).add(mainLabels.get(i));
        }
        panels.forEach(this::add);
        GuiLibrary.setPanelStyle(cards);
        cards.getStyle().getBackground().setColor(new Vector4f(0, 0, 0, 0));
        panels.get(3).add(cards);
        mainLabels.get(2).getListenerMap().addListener(MouseClickEvent.class, (MouseClickEventListener) event -> {
            if (event.getAction() == MouseClickEvent.MouseClickAction.RELEASE) {
                if (panelStack.isEmpty())
                    call(optionPanel, true);
            }
        });

        skipButton = new Button("Skip Phase", 25, 15, 150f, 70f);
        GuiLibrary.setButtonStyle(skipButton);
        skipButton.getStyle().setFont("roboto-regular");
        skipButton.getListenerMap().addListener(MouseClickEvent.class, (MouseClickEventListener) event -> {
            if (event.getAction() == MouseClickEvent.MouseClickAction.RELEASE) {
                Main.logic.player.switchTurn();
            }
        });
    }

    private void createCursor() {
        Label cursor = new Label("\uE145");
        cursor.getStyle().setFont(FontRegistry.MATERIAL_ICONS_REGULAR);
        cursor.getStyle().setFontSize(36.0f);
        cursor.getStyle().setVerticalAlign(VerticalAlign.MIDDLE);
        cursor.getStyle().setHorizontalAlign(HorizontalAlign.CENTER);
        cursor.getStyle().setTextColor(ColorConstants.white());
        cursor.setPosition(0, 0);
        cursor.setSize(width, height);
        this.add(cursor);
    }

    public GUI(int width, int height) {
        super(0, 0, width, height);
        this.width = width;
        this.height = height;
        this.panelStack = new ArrayList<>();
        FontRegistry.registerFont("YaHei Mono", "resources/fonts/Microsoft YaHei Mono.ttf");
        FontRegistry.registerFont("Impact", "resources/fonts/impact.ttf");
        FontRegistry.registerFont("YaHei", "resources/fonts/msyh.ttc");
        FontRegistry.registerFont("XinWei", "resources/fonts/STXINWEI.TTF");
        subPanel = new SubPanel("Sub Menu");
        optionPanel = new OptionPanel();
        cards = new CardStack();
        createStyle();
        createCursor();
        createUtilityPanels();
        debugPanel = new DebugPanel(this);
        panels.get(4).clearChildComponents();
        if (Main.logic.player.canExecute) {
            panels.get(4).add(skipButton);
        } else {
            panels.get(4).add(mainLabels.get(4));
        }
        logon = false;
    }

    public void resize(int w, int h) {
        this.width = w;
        this.height = h;
        GuiLibrary.width = w;
        GuiLibrary.height = h;
        this.setSize(w, h);
    }

    public boolean pause() {
        if (lock)
            return false;
        if (panelStack.isEmpty()) {
            call(optionPanel, true);
            return true;
        } else {
            PanelInstance instance = panelStack.get(panelStack.size() - 1);
            close(instance, true);
            return false;
        }
    }

    public void update() {
        mainLabels.get(1).setTextState(new TextState(InfoLibrary.getGUIStatus()));
        mainLabels.get(0).setTextState(new TextState(InfoLibrary.getTurnStatus()));
        cards.update();
    }
}