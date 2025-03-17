package sdu.revolution.client.engine.gui;

import com.spinyowl.legui.DefaultInitializer;
import com.spinyowl.legui.animation.Animator;
import com.spinyowl.legui.animation.AnimatorProvider;
import com.spinyowl.legui.component.Component;
import com.spinyowl.legui.component.Frame;
import com.spinyowl.legui.component.Label;
import com.spinyowl.legui.style.Style;
import com.spinyowl.legui.style.border.SimpleLineBorder;
import com.spinyowl.legui.style.color.ColorConstants;
import com.spinyowl.legui.system.context.Context;
import com.spinyowl.legui.system.renderer.Renderer;
import org.joml.Vector2i;
import org.joml.Vector4f;
import org.lwjgl.glfw.GLFW;
import sdu.revolution.client.Main;
import sdu.revolution.client.engine.main.IGuiInstance;
import sdu.revolution.client.engine.main.Window;
import sdu.revolution.client.engine.scene.Scene;
import sdu.revolution.client.engine.gui.screen.LoginScreen;

import static com.spinyowl.legui.component.optional.align.HorizontalAlign.CENTER;
import static com.spinyowl.legui.component.optional.align.VerticalAlign.MIDDLE;
import static org.lwjgl.glfw.GLFW.glfwGetFramebufferSize;

public class MainMenu implements IGuiInstance {

    private int width, height;
    private Frame frame;
    private GUI gui;
    private Context context;
    private Renderer renderer;
    private DefaultInitializer initializer;
    private Animator animator;

    public GUI getGui() {
        return gui;
    }

    private void createGuiElements(Frame frame, int w, int h) {
        GuiLibrary.width = w;
        GuiLibrary.height = h;
        gui = new GUI(w, h);
        gui.setFocusable(false);
        gui.getStyle().setMinWidth(100F);
        gui.getStyle().setMinHeight(100F);
        gui.getStyle().getFlexStyle().setFlexGrow(1);
        gui.getStyle().setPosition(Style.PositionType.RELATIVE);
        gui.getStyle().getBackground().setColor(ColorConstants.transparent());

        frame.getContainer().getStyle().setDisplay(Style.DisplayType.FLEX);
        frame.getContainer().add(gui);
    }

    public void reload() {
        if (renderer != null) {
            renderer.destroy();
        }
        int[] arrWidth = new int[1];
        int[] arrHeight = new int[1];
        GLFW.glfwGetFramebufferSize(Main.getEngine().getWindow().getHandle(), arrWidth, arrHeight);
        width = arrWidth[0];
        height = arrHeight[0];
        gui.clearChildComponents();
        gui.resize(width, height);
        frame = new Frame(width, height);
        createGuiElements(frame, width, height);
        Label label = new Label(width / 2.0f - 200.0f, 50, 400, 100);
        label.getTextState().setText("Your layout has been reloaded.");
        label.getStyle().setTextColor(new Vector4f(1.0f, 1.0f, 1.0f, 0.0f));
        label.getStyle().setBorder(new SimpleLineBorder());
        label.getStyle().setFont("Impact");
        label.getStyle().setFontSize(40f);
        label.getStyle().setHorizontalAlign(CENTER);
        label.getStyle().setVerticalAlign(MIDDLE);
        gui.add(label);
        initializer = new DefaultInitializer(Main.getEngine().getWindow().getHandle(), frame);
        renderer = initializer.getRenderer();
        animator = AnimatorProvider.getAnimator();
        renderer.initialize();
        context = initializer.getContext();
        gui.panelStack.clear();
        Main.Logger.info(this, "Layout Reloaded");
        new Thread(() -> {
            try {
                for (int i = 1; i <= 100; i += 10) {
                    label.getStyle().setTextColor(new Vector4f(1.0f, 1.0f, 1.0f, i / 100.0f));
                    Thread.sleep(10);
                }
                Thread.sleep(1000);
                for (int i = 1; i <= 100; i += 10) {
                    label.getStyle().setTextColor(new Vector4f(1.0f, 1.0f, 1.0f, (100 - i) / 100.0f));
                    Thread.sleep(10);
                }
                gui.remove(label);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }).start();
    }

    public MainMenu() {
        int[] arrWidth = new int[1];
        int[] arrHeight = new int[1];
        GLFW.glfwGetFramebufferSize(Main.getEngine().getWindow().getHandle(), arrWidth, arrHeight);
        width = arrWidth[0];
        height = arrHeight[0];
        frame = new Frame(width, height);
        createGuiElements(frame, width, height);
        initializer = new DefaultInitializer(Main.getEngine().getWindow().getHandle(), frame);
        renderer = initializer.getRenderer();
        animator = AnimatorProvider.getAnimator();
        renderer.initialize();
        context = initializer.getContext();
    }

    @Override
    public void drawGui() {
    }

    @Override
    public boolean handleGuiInput(Scene scene, Window window) {
        return false;
    }

    @Override
    public void update() {
        gui.update();
        context.updateGlfwWindow();
        Vector2i windowSize = context.getFramebufferSize();

        // We need to relayout components.
//        if (gui.getGenerateEventsByLayoutManager().isChecked()) {
//            LayoutManager.getInstance().layout(frame, context);
//        } else {
//            LayoutManager.getInstance().layout(frame);
//        }

        // render frame
        renderer.render(frame, context);

        // poll events to callbacks
        animator.runAnimations();

        // Now we need to handle events. Firstly we need to handle system events.
        // And we need to know to which frame they should be passed.
        initializer.getSystemEventProcessor().processEvents(frame, context);

        // When system events are translated to GUI events we need to handle them.
        // This event processor calls listeners added to ui components
        initializer.getGuiEventProcessor().processEvents();

        // check toggle fullscreen flag and execute.

        updateElement();
    }

    private void updateElement() {
        if (context != null) {
            Component mouseTargetGui = context.getMouseTargetGui();
//            gui.getMouseTargetLabel().getTextState().setText("-> " + (mouseTargetGui == null ? null : mouseTargetGui.getClass().getSimpleName()));

            Component focusedGui = context.getFocusedGui();
//            gui.getFocusedGuiLabel().getTextState().setText("-> " + (focusedGui == null ? null : focusedGui.getClass().getSimpleName()));
        }
    }

    public void cleanup() {
        renderer.destroy();
    }

    public void resize(int w, int h) {
        width = w;
        height = h;
    }

    public boolean callPause() {
        return gui.pause();
    }

    public void showLogin() {
        LoginScreen loginScreen = new LoginScreen();
        gui.add(loginScreen);
    }
}