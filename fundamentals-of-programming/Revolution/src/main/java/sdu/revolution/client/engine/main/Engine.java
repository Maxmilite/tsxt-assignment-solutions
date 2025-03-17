package sdu.revolution.client.engine.main;

import sdu.revolution.client.Main;
import sdu.revolution.client.engine.graph.Render;
import sdu.revolution.client.engine.scene.Scene;

import java.io.IOException;

import static org.lwjgl.glfw.GLFW.*;

public class Engine {

    public static final int TARGET_UPS = 60;
    // Running on 60 fps
    private final IAppLogic appLogic;
    private final Window window;
    private final Render render;
    private boolean running;
    private final Scene scene;
    private final int targetFps;
    private final int targetUps;
    private int varWidth, varHeight;

    public Window getWindow() {
        return window;
    }

    public Scene getScene() {
        return scene;
    }

    public Engine(String windowTitle, Window.WindowOptions opts, IAppLogic appLogic) {
        window = new Window(windowTitle, opts, () -> {
            resize();
            return null;
        });
        targetFps = opts.fps;
        targetUps = opts.ups;
        this.appLogic = appLogic;
        render = new Render(window);
        scene = new Scene(window.getWidth(), window.getHeight());
        appLogic.init(window, scene, render);
        running = true;
        int[] arrWidth = new int[1];
        int[] arrHeight = new int[1];
        glfwGetFramebufferSize(window.getHandle(), arrWidth, arrHeight);
        varWidth = arrWidth[0];
        varHeight = arrHeight[0];
    }

    private void cleanup() throws IOException {
        appLogic.cleanup();
        render.cleanup();
        scene.cleanup();
        window.cleanup();
        Main.menu.cleanup();
        Main.Logger.info("Program will exit normally.");
        glfwDestroyWindow(window.getHandle());
        glfwTerminate();
    }

    private void resize() {
        int width = window.getWidth();
        int height = window.getHeight();
        scene.resize(width, height);
        render.resize(width, height);
        int[] arrWidth = new int[1];
        int[] arrHeight = new int[1];
        glfwGetFramebufferSize(window.getHandle(), arrWidth, arrHeight);
        width = arrWidth[0];
        height = arrHeight[0];
        varWidth = width;
        varHeight = height;
        Main.menu.resize(width, height);
    }

    private void run() throws IOException {
        long initialTime = System.currentTimeMillis();
        float timeU = 1000.0f / targetUps;
        float timeR = targetFps > 0 ? 1000.0f / targetFps : 0;
        float deltaUpdate = 0;
        float deltaFps = 0;
        IGuiInstance iGuiInstance = scene.getGuiInstance();
        long updateTime = initialTime;
        while (running && !window.windowShouldClose()) {
            window.pollEvents();

            long now = System.currentTimeMillis();
            deltaUpdate += (now - initialTime) / timeU;
            deltaFps += (now - initialTime) / timeR;

            if (targetFps <= 0 || deltaFps >= 1) {
                boolean inputConsumed = iGuiInstance != null && iGuiInstance.handleGuiInput(scene, window);
                appLogic.input(window, scene, now - initialTime, inputConsumed);
            }

            if (deltaUpdate >= 1) {
                long diffTimeMillis = now - updateTime;
                appLogic.update(window, scene, diffTimeMillis);
                updateTime = now;
                deltaUpdate--;
            }

            if (targetFps <= 0 || deltaFps >= 1) {
                render.render(window, scene);
                if (Main.menu != null) {
                    Main.menu.update();
                }
                deltaFps--;
                window.update();
            }
            initialTime = now;

            int[] arrWidth = new int[1];
            int[] arrHeight = new int[1];
            glfwGetFramebufferSize(window.getHandle(), arrWidth, arrHeight);
            int width = arrWidth[0];
            int height = arrHeight[0];
            if (width != varWidth || height != varHeight) {
                resize();
                varWidth = width;
                varHeight = height;
            }
        }

        cleanup();
    }

    public void start() throws IOException {
        running = true;
        run();
    }

    public void stop() {
        running = false;
    }

}