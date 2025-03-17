package sdu.revolution.client.engine.gui;

import com.spinyowl.legui.component.*;
import com.spinyowl.legui.component.optional.align.HorizontalAlign;
import com.spinyowl.legui.component.optional.align.VerticalAlign;
import com.spinyowl.legui.image.loader.ImageLoader;
import com.spinyowl.legui.style.border.SimpleLineBorder;
import com.spinyowl.legui.style.color.ColorConstants;
import com.spinyowl.legui.style.color.ColorUtil;
import com.spinyowl.legui.style.font.FontRegistry;
import org.joml.Vector2f;
import org.joml.Vector4f;
import sdu.revolution.client.Main;
import sdu.revolution.client.engine.gui.panels.PromptPanel;

public class GuiLibrary {
    public static int width, height;
    public static final int TRANSITION_BACKGROUND = 1, TRANSITION_FONT = 2, TRANSITION_REMOVAL = 4, TRANSITION_IMAGE = 8;
    public enum Color {
        AQUA
    }
    public static Vector4f getColor(Color color) {
        if (color == Color.AQUA) {
            return ColorUtil.rgba(0, 255, 255, 1f);
        }
        // White
        return ColorUtil.rgba(255, 255, 255, 1f);
    }
    public static void setAlign(Component component, VerticalAlign verticalAlign, HorizontalAlign horizontalAlign) {
        component.getStyle().setVerticalAlign(verticalAlign);
        component.getStyle().setHorizontalAlign(horizontalAlign);
    }
    public static void setFont(Component component, String s, float size) {
        component.getStyle().setFontSize(size);
        component.getStyle().setFont(s);
    }
    public static void setPanelStyle(Panel panel) {
        panel.getStyle().getBackground().setColor(new Vector4f(0.0f, 0.0f, 0.0f, 0.75f));
        panel.getStyle().setBorder(new SimpleLineBorder(new Vector4f(1f, 1f, 1f, 1f), 1));
    }
    public static void setCloseButtonStyle(Button close, float size) {
        close.getStyle().setTextColor(ColorUtil.rgba(0, 255, 255, 1f));
        close.getStyle().setHorizontalAlign(HorizontalAlign.CENTER);
        close.getStyle().setVerticalAlign(VerticalAlign.MIDDLE);
        close.getStyle().setFontSize(size);
        close.getStyle().setFont(FontRegistry.MATERIAL_ICONS_REGULAR);
        close.getStyle().getBackground().setColor(new Vector4f(ColorConstants.TRANSPARENT));
        close.getHoveredStyle().getBackground().setColor(new Vector4f(ColorConstants.TRANSPARENT));
        close.getHoveredStyle().setTextColor(new Vector4f(1f, 1f, 0f, 1f));
        close.getPressedStyle().getBackground().setColor(new Vector4f(ColorConstants.TRANSPARENT));
        close.getPressedStyle().setTextColor(new Vector4f(ColorConstants.WHITE));
    }

    public static void setButtonStyle(Button button) {
        button.getStyle().setTextColor(ColorUtil.rgba(0, 255, 255, 1f));
        button.getStyle().setHorizontalAlign(HorizontalAlign.CENTER);
        button.getStyle().setVerticalAlign(VerticalAlign.MIDDLE);
        button.getStyle().setFontSize(20f);
        button.getStyle().setFont("Impact");
        button.getStyle().getBackground().setColor(0.3f, 0.3f, 0.3f, 1.0f);
    }

    public static void runSlide(Component component, int duration, Vector2f targetSize, Vector2f targetPosition) {
        Main.menu.getGui().lock = true;
//        component.getChildComponents().forEach((e) -> runSlide(e, duration, targetSize, targetPosition));
        float targetWidth = targetSize.x, targetHeight = targetSize.y;
        float targetPositionX = targetPosition.x, targetPositionY = targetPosition.y;
        float currentWidth = component.getSize().x, currentHeight = component.getSize().y;
        float currentPositionX = component.getPosition().x, currentPositionY = component.getPosition().y;
        new Thread(() -> {
            try {
                Vector2f deltaSize = new Vector2f(
                        (10.0f / duration) * (targetWidth - currentWidth),
                        (10.0f / duration) * (targetHeight - currentHeight)
                );
                Vector2f deltaPosition = new Vector2f(
                        (10.0f / duration) * (targetPositionX - currentPositionX),
                        (10.0f / duration) * (targetPositionY - currentPositionY)
                );
                for (int i = 1; i <= duration / 10; ++i) {
                    component.setSize(component.getSize().add(deltaSize));
                    component.setPosition(component.getPosition().add(deltaPosition));
                    Thread.sleep(10);
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }).start();
        new Thread(() -> {
            try {
                Thread.sleep(duration + 200);
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
            Main.menu.getGui().lock = false;
        }).start();
    }

    public static void runTransition(Component component, int fadeInTime, int duration, int fadeOutTime, int opts) {
        Main.menu.getGui().lock = true;
        component.getChildComponents().forEach((e) -> runTransition(e, fadeInTime, duration, fadeOutTime, opts & 3));
        if ((opts & TRANSITION_BACKGROUND) != 0) {
            Vector4f vec = new Vector4f(component.getStyle().getBackground().getColor());
            var ref = new Object() {
                float thickness = -1.0F;
            };
            if (component.getStyle().getBorder() != null) {
                ref.thickness = ((SimpleLineBorder) component.getStyle().getBorder()).getThickness();
            }
            float limit = vec.w;
            new Thread(() -> {
                try {
                    for (int i = 1; i <= fadeInTime / 10; ++i) {
                        vec.w = (10.0f * i / fadeInTime) * limit;
                        component.getStyle().getBackground().setColor(vec);
                        if (component.getStyle().getBorder() != null)
                            ((SimpleLineBorder) component.getStyle().getBorder()).setThickness((10.0f * i / fadeInTime) * ref.thickness);
                        Thread.sleep(10);
                    }
                    Thread.sleep(duration);
                    for (int i = 1; i <= fadeOutTime / 10; ++i) {
                        float v = 10.0f * (fadeOutTime / 10 - i) / fadeOutTime;
                        vec.w = v * limit;
                        component.getStyle().getBackground().setColor(vec);
                        if (component.getStyle().getBorder() != null)
                            ((SimpleLineBorder) component.getStyle().getBorder()).setThickness(
                                    v * ref.thickness
                            );
                        Thread.sleep(10);
                        if (i == fadeOutTime / 10 && (opts & TRANSITION_REMOVAL) != 0) {
                            Main.menu.getGui().remove(component);
                        }
                    }
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                vec.w = limit;
                component.getStyle().getBackground().setColor(vec);
                if (component.getStyle().getBorder() != null)
                    ((SimpleLineBorder) component.getStyle().getBorder()).setThickness(ref.thickness);
            }).start();
        }
        if ((opts & TRANSITION_FONT) != 0) {
            Vector4f vec = new Vector4f(component.getStyle().getTextColor());
            float limit = vec.w;
            new Thread(() -> {
                try {
                    for (int i = 1; i <= fadeInTime / 10; ++i) {
                        vec.w = (10.0f * i / fadeInTime) * limit;
                        component.getStyle().setTextColor(vec);
                        Thread.sleep(10);
                    }
                    Thread.sleep(duration);
                    for (int i = 1; i <= fadeOutTime / 10; ++i) {
                        vec.w = (10.0f * (fadeOutTime / 10 - i) / fadeOutTime) * limit;
                        component.getStyle().setTextColor(vec);
                        Thread.sleep(10);
                        if (i == fadeOutTime / 10 && (opts & TRANSITION_REMOVAL) != 0) {
                            Main.menu.getGui().remove(component);
                        }
                    }
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                vec.w = limit;
                component.getStyle().setTextColor(vec);
            }).start();
        }
        new Thread(() -> {
            try {
                Thread.sleep(fadeInTime + duration + fadeOutTime + 200);
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
            Main.menu.getGui().lock = false;
        }).start();
    }

    @SuppressWarnings("rawtypes, unchecked")
    public static void prompt(String content, Runnable yesFunction, Runnable noFunction) {
        if (Main.menu.getGui() == null)
                return;
        PromptPanel promptPanel = new PromptPanel(content, yesFunction, noFunction);
        promptPanel.call();
        Main.menu.getGui().panelStack.add(promptPanel);
    }

    public static ImageView createCardIcon(String path) {
        // com/spinyowl/legui/demo/1.jpg
        ImageView imageView = new ImageView(ImageLoader.loadImage(path));
        imageView.setPosition(3, 0);
        imageView.setSize(64, 64);
        imageView.getStyle().setBorderRadius(0f);
        return imageView;
    }
}
