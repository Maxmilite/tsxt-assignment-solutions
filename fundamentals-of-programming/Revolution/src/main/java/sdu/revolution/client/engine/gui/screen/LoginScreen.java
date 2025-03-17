package sdu.revolution.client.engine.gui.screen;

import com.spinyowl.legui.component.*;
import com.spinyowl.legui.event.MouseClickEvent;
import com.spinyowl.legui.image.loader.ImageLoader;
import com.spinyowl.legui.listener.MouseClickEventListener;
import com.spinyowl.legui.style.border.SimpleLineBorder;
import com.spinyowl.legui.style.color.ColorConstants;
import org.joml.Vector4f;
import sdu.revolution.client.Main;
import sdu.revolution.client.engine.gui.GuiLibrary;

import java.io.IOException;

import static com.spinyowl.legui.component.optional.align.HorizontalAlign.*;

public class LoginScreen extends Panel {
    public LoginScreen() {
        super(0, 0, GuiLibrary.width, GuiLibrary.height);
        init();
    }

    public boolean login(String x) throws IOException {
        x = x.trim();
        if (x.isEmpty())
            return false;
        if (!Main.networkClient.registerOnline(x))
            return false;
        Main.logic.player.name = x;
        return true;
    }

    public void close() {
        GuiLibrary.runTransition(this, 0, 0, 1000, GuiLibrary.TRANSITION_BACKGROUND | GuiLibrary.TRANSITION_FONT | GuiLibrary.TRANSITION_IMAGE);
        new Thread(() -> {
            try {
                Thread.sleep(1500);
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
            Main.menu.getGui().remove(this);
            Main.menu.getGui().logon = true;
        }).start();
    }

    public void init() {
        this.getStyle().getBackground().setColor(new Vector4f(ColorConstants.BLACK));
        ImageView title = new ImageView(ImageLoader.loadImage("resources/textures/title.png"));
        title.getStyle().getBackground().setColor((Vector4f) ColorConstants.TRANSPARENT);
        title.setPosition(GuiLibrary.width / 2 - 256, 200);
        title.setSize(512, 256);
        title.getStyle().setBorder(new SimpleLineBorder());
        title.setFocusable(false);
        this.add(title);

        Label label = new Label("Enter your username:", GuiLibrary.width / 2 - 200, GuiLibrary.height - 450, GuiLibrary.width, 50);
        label.getStyle().setFont("Impact");
        label.getStyle().setFontSize(36f);
        label.getStyle().setTextColor(new Vector4f(ColorConstants.WHITE));
        this.add(label);

        TextInput textInput = new TextInput(GuiLibrary.width / 2 - 200, GuiLibrary.height - 400, 400, 60);
        textInput.getStyle().setHorizontalAlign(CENTER);
        textInput.getStyle().setFont("roboto-regular");
        textInput.getStyle().setFontSize(28f);
        this.add(textInput);

        Button button = new Button("Login", GuiLibrary.width / 2 - 150, GuiLibrary.height - 280, 300, 60);
        button.getStyle().setHorizontalAlign(CENTER);
        button.getStyle().setFont("Impact");
        button.getStyle().setFontSize(28f);
        button.getListenerMap().addListener(MouseClickEvent.class, (MouseClickEventListener) event -> {
            if (event.getAction() == MouseClickEvent.MouseClickAction.RELEASE) {
                try {
                    if (login(textInput.getTextState().getText())) {
                        this.remove(textInput);
                        this.remove(label);
                        this.remove(button);
                        new Thread(() -> {
                            try {
                                Label fail = new Label("Succeed to Login. Prepare to enter the battlefield.", 0, GuiLibrary.height - 450, GuiLibrary.width, 72);
                                fail.getStyle().setFont("roboto-regular");
                                fail.getStyle().setFontSize(36f);
                                fail.getStyle().setHorizontalAlign(CENTER);
                                this.add(fail);
                                for (int i = 1; i <= 500; i += 10) {
                                    fail.getStyle().setTextColor(new Vector4f(1.0f, 1.0f, 1.0f, i / 100.0f));
                                    Thread.sleep(10);
                                }
                                Thread.sleep(2000);
                                this.close();
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }
                        }).start();
                    } else {
                        new Thread(() -> {
                            try {
                                Label fail = new Label("Failed to login: Same name with existing players.", 0, GuiLibrary.height - 180, GuiLibrary.width, 50);
                                fail.getStyle().setFont("roboto-regular");
                                fail.getStyle().setFontSize(24f);
                                fail.getStyle().setHorizontalAlign(CENTER);
                                fail.getStyle().setTextColor(new Vector4f(ColorConstants.WHITE));
                                this.add(fail);
                                for (int i = 1; i <= 100; i += 10) {
                                    fail.getStyle().setTextColor(new Vector4f(1.0f, 1.0f, 1.0f, i / 100.0f));
                                    Thread.sleep(10);
                                }
                                Thread.sleep(1000);
                                for (int i = 1; i <= 100; i += 10) {
                                    fail.getStyle().setTextColor(new Vector4f(1.0f, 1.0f, 1.0f, (100 - i) / 100.0f));
                                    Thread.sleep(10);
                                }
                                this.remove(fail);
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }
                        }).start();
                    }
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }
            }
        });
        this.add(button);
    }
}
