package sdu.revolution.client.engine.main;

import sdu.revolution.client.engine.scene.Scene;

public interface IGuiInstance {
    void drawGui();

    boolean handleGuiInput(Scene scene, Window window);
    void update();
}