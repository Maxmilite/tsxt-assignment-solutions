package sdu.revolution.client;

import org.joml.Vector2f;
import sdu.revolution.client.engine.graph.Render;
import sdu.revolution.client.engine.gui.MainMenu;
import sdu.revolution.client.engine.main.*;
import sdu.revolution.client.engine.model.ItemManager;
import sdu.revolution.client.engine.scene.Camera;
import sdu.revolution.client.engine.scene.Scene;
import sdu.revolution.client.engine.scene.SkyBox;
import sdu.revolution.client.engine.scene.lights.SceneLights;
import sdu.revolution.client.logic.LogicManager;
import sdu.revolution.client.network.RevolutionClient;

import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;

import static org.lwjgl.glfw.GLFW.*;


public class Main implements IAppLogic {
    public static class Logger {
        public static void info(Object x) {
            System.out.println("\033[0m[\033[31mRevolution\033[0m] \033[33m" + new Date() + "\033[0m | Info : \033[32m" + x.toString() + "\033[0m");
        }

        public static void info(Object object, Object x) {
            System.out.println("\033[0m[\033[31mRevolution\033[0m] \033[33m" + new Date() + "\033[0m | Info from " + object.getClass().getSimpleName() + " : \033[32m" + x.toString() + "\033[0m");
        }

        public static void info(Class<?> object, Object x) {
            System.out.println("\033[0m[\033[31mRevolution\033[0m] \033[33m" + new Date() + "\033[0m | Info from " + object.getSimpleName() + " : \033[32m" + x.toString() + "\033[0m");
        }
    }

    public static Main INSTANCE;
    private static final float MOUSE_SENSITIVITY = 0.1f;
    private static final float MOVEMENT_SPEED = 0.005f;
    private static final float SPRINT_AMPLIFIER = 2.0f;
    public static boolean isControlled;
    private static boolean isAltPressed;

    private static Engine engine;
    public static MainMenu menu;
    public static LogicManager logic;
    public static RevolutionClient networkClient;

    public static void main(String[] args) throws IOException, InterruptedException {

        System.setProperty("joml.nounsafe", Boolean.TRUE.toString());
        System.setProperty("java.awt.headless", Boolean.TRUE.toString());

        Logger.info("Revolution: Thanks for your playing.");
        SimpleDateFormat format = new SimpleDateFormat();
        format.applyPattern("yyyy-MM-dd HH:mm:ss");
        Logger.info("Game started at " + format.format(new Date()) + ".");

        Utils.loadConfig();
        networkClient = new RevolutionClient();

        isControlled = false;
        isAltPressed = false;

        ItemManager.init();
        INSTANCE = new Main();
        Window.WindowOptions options = new Window.WindowOptions();
        logic = new LogicManager();
        engine = new Engine(Utils.getTitle(), options, INSTANCE);
        menu = new MainMenu();
        menu.showLogin();
        engine.start();
    }

    public static Engine getEngine() {
        return engine;
    }

    @Override
    public void cleanup() {
        try {
            Utils.saveConfig();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        try {
            networkClient.close();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    @Override
    public void init(Window window, Scene scene, Render render) {
        long startTime = System.currentTimeMillis();
        ItemManager.init(window, scene, render);

        SceneLights sceneLights = new SceneLights();
        sceneLights.getAmbientLight().setIntensity(0.3f);
        sceneLights.getAmbientLight().setColor(0.5f, 0.5f, 0.5f);
        sceneLights.getDirLight().setPosition(-0.9f, 0.6f, 0.4f);
        sceneLights.getDirLight().setIntensity(0.6f);
        scene.setSceneLights(sceneLights);


        SkyBox skyBox = new SkyBox(Utils.getResourceDir() + "/models/skybox/skybox.obj", scene.getTextureCache());
        skyBox.getSkyBoxEntity().setScale(500);
        scene.setSkyBox(skyBox);
        glfwSetCursorPos(window.getHandle(), window.getWidth() >> 1, window.getHeight() >> 1);
        scene.getCamera().setPosition(0f, 2.5f, 2f);

        long endTime = System.currentTimeMillis();
        Logger.info(String.format("Graphics System initialized. Using %.2f seconds.", (endTime - startTime) / 1000.0));
    }

    @Override
    public void input(Window window, Scene scene, long diffTimeMillis, boolean inputConsumed) {
        if (isControlled) {
            // Nothing to be done yet
            ItemManager.input(window, scene, diffTimeMillis);
            float move = diffTimeMillis * MOVEMENT_SPEED;
            if (window.isKeyPressed(GLFW_KEY_LEFT_CONTROL)) move *= SPRINT_AMPLIFIER;
            Camera camera = scene.getCamera();
            if (window.isKeyPressed(GLFW_KEY_W)) {
                camera.moveForward(move);
            } else if (window.isKeyPressed(GLFW_KEY_S)) {
                camera.moveBackwards(move);
            }
            if (window.isKeyPressed(GLFW_KEY_A)) {
                camera.moveLeft(move);
            } else if (window.isKeyPressed(GLFW_KEY_D)) {
                camera.moveRight(move);
            }
            if (window.isKeyPressed(GLFW_KEY_UP) || window.isKeyPressed(GLFW_KEY_SPACE)) {
                camera.moveUp(move);
            } else if (window.isKeyPressed(GLFW_KEY_DOWN) || window.isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
                camera.moveDown(move);
            }

            MouseInput mouseInput = window.getMouseInput();

            Vector2f displVec = mouseInput.getDisplVec();
            camera.addRotation((float) Math.toRadians(displVec.x * MOUSE_SENSITIVITY), (float) Math.toRadians(displVec.y * MOUSE_SENSITIVITY));
        }

        if (glfwGetKey(window.getHandle(), GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS) {
            isAltPressed = true;
        } else if (isAltPressed) {
            isControlled = !isControlled;
            isAltPressed = false;
        }


        // TODO: Use stack to control the sequence of panels
        // Finished on 2023-01-12 23:05
        if (window.isKeyPressed(GLFW_KEY_ESCAPE)) {
            if (menu.callPause()) {
                isControlled = false;
            }
        }

    }

    @Override
    public void update(Window window, Scene scene, long diffTimeMillis) {
        ItemManager.update(window, scene, diffTimeMillis);
        menu.update();
        if (isControlled) {
            if (glfwGetInputMode(window.getHandle(), GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
                glfwSetInputMode(window.getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        } else {
            if (glfwGetInputMode(window.getHandle(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
                glfwSetInputMode(window.getHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                glfwSetCursorPos(window.getHandle(), window.getWidth() >> 1, window.getHeight() >> 1);
            }
        }
    }
}
