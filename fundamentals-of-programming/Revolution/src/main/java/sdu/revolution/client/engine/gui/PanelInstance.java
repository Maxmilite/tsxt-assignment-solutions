package sdu.revolution.client.engine.gui;

import com.spinyowl.legui.component.Panel;

public class PanelInstance extends Panel {
    protected boolean isWindow;
    protected boolean isOpen;
    public int width, height;
    public PanelInstance() {
        super();
        this.isWindow = true;
        this.isOpen = false;
        this.width = GuiLibrary.width;
        this.height = GuiLibrary.height;
        this.init();
    }
    public void init() {}
    public boolean isWindow() {
        return isWindow;
    }

    public void setWindow(boolean window) {
        isWindow = window;
    }

    public boolean isOpen() {
        return isOpen;
    }

    public void setOpen(boolean open) {
        isOpen = open;
    }

    public void close() {}
    public void call() {}
}
