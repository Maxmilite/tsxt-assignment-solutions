package sdu.revolution.client.engine.main;

import sdu.revolution.client.engine.graph.Render;
import sdu.revolution.client.engine.scene.Scene;

public interface IAppLogic {
    void cleanup();

    void init(Window window, Scene scene, Render render);

    void input(Window window, Scene scene, long diffTimeMillis, boolean inputConsumed);

    void update(Window window, Scene scene, long diffTimeMillis);
}
