package sdu.revolution.client.engine.model;

import org.joml.Vector2d;
import org.joml.Vector3f;
import sdu.revolution.client.Main;
import sdu.revolution.client.engine.model.items.BobModel;
import sdu.revolution.client.engine.scene.CameraBoxSelectionDetector;
import sdu.revolution.client.engine.scene.MouseBoxSelectionDetector;
import sdu.revolution.client.engine.graph.Render;
import sdu.revolution.client.engine.main.Window;
import sdu.revolution.client.engine.model.items.DirtBlock;
import sdu.revolution.client.engine.model.items.GrassBlock;
import sdu.revolution.client.engine.model.terrain.Plain;
import sdu.revolution.client.engine.scene.Scene;

import java.util.ArrayList;
import java.util.List;

import static org.lwjgl.glfw.GLFW.*;

public class ItemManager {
    public static final int MAP_SIZE = 5;
    public static final int LOWEST_HEIGHT = -1;
    public static List<Item> list;
    public static Plain plain;
    public static MouseBoxSelectionDetector mouseDetector;
    public static CameraBoxSelectionDetector cameraDetector;
    public static void init() {
        list = new ArrayList<>();
        plain = new Plain(MAP_SIZE);
        for (int i = -MAP_SIZE; i <= MAP_SIZE; ++i)
            for (int j = -MAP_SIZE; j <= MAP_SIZE; ++j)  {
                int height = plain.getHeight(i, j);
                list.add(new GrassBlock(new Vector3f(i, height, j)));
                for (int k = height - 1; k >= LOWEST_HEIGHT; --k) {
                    list.add(new DirtBlock(new Vector3f(i, k, j)));
                }
            }
        list.add(new BobModel(new Vector3f(0, 5, 0)));
        plain.cleanup();
        cameraDetector = new CameraBoxSelectionDetector();
        mouseDetector = new MouseBoxSelectionDetector();
    }
    public static void init(Window window, Scene scene, Render render) {
        for (var i : list) {
            i.init(window, scene, render);
        }
    }
    public static void input(Window window, Scene scene, long diffTimeMillis) {
        for (var i : list) {
            i.input(window, scene, diffTimeMillis);
        }
    }
    public static void update(Window window, Scene scene, long diffTimeMillis) {
        double[] arrX = new double[1];
        double[] arrY = new double[1];
        glfwGetCursorPos(window.getHandle(), arrX, arrY);
        Vector2d currentPos = new Vector2d(arrX[0], arrY[0]);
        if (!Main.menu.getGui().panelStack.isEmpty()
                || glfwGetWindowAttrib(window.getHandle(), GLFW_HOVERED) == 0
        || !Main.menu.getGui().logon) {
            cameraDetector.cancelSelection(list);
        } else if (Main.isControlled) {
            cameraDetector.selectItem(list, Main.getEngine().getScene().getCamera());
        } else {
            mouseDetector.selectItem(list, window, currentPos, Main.getEngine().getScene().getCamera());
        }
        for (var i : list) {
            i.update(window, scene, diffTimeMillis);
        }
    }
}
