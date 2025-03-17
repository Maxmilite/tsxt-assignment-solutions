package sdu.revolution.client.engine.main;

import org.joml.Vector2f;

import static org.lwjgl.glfw.GLFW.*;

public class MouseInput {

    private Vector2f currentPos;
    private Vector2f displVec;
    public boolean inWindow;
    private boolean leftButtonPressed;
    private Vector2f previousPos;
    private boolean rightButtonPressed;
    private long handle;

    public MouseInput(long windowHandle) {
        handle = windowHandle;
        previousPos = new Vector2f(1919810, 1919810);
        currentPos = new Vector2f();
        displVec = new Vector2f();
        leftButtonPressed = false;
        rightButtonPressed = false;
        inWindow = true;

//        glfwSetCursorPosCallback(windowHandle, (handle, xpos, ypos) -> {
//            currentPos.x = (float) xpos;
//            currentPos.y = (float) ypos;
//        });
        glfwSetCursorEnterCallback(windowHandle, (handle, entered) -> inWindow = entered);


//        glfwSetMouseButtonCallback(windowHandle, (handle, button, action, mode) -> {
//            leftButtonPressed = button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS;
//            rightButtonPressed = button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS;
//        });
    }

    public Vector2f getCurrentPos() {
        return currentPos;
    }

    public Vector2f getDisplVec() {
        return displVec;
    }

    private void updateInputStatus() {
        double[] arrX = new double[1];
        double[] arrY = new double[1];
        glfwGetCursorPos(handle, arrX, arrY);
        currentPos = new Vector2f((float) arrX[0], (float) arrY[0]);
        glfwSetCursorEnterCallback(handle, (handle, entered) -> inWindow = entered);
        leftButtonPressed = glfwGetMouseButton(handle, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;
        rightButtonPressed = glfwGetMouseButton(handle, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS;
    }

    public void input() {

        updateInputStatus();

        displVec.x = 0;
        displVec.y = 0;
        if (previousPos.x != 1919810 && previousPos.y != 1919810 && inWindow) {
            double deltax = currentPos.x - previousPos.x;
            double deltay = currentPos.y - previousPos.y;
            boolean rotateX = deltax != 0;
            boolean rotateY = deltay != 0;
            if (rotateX) {
                displVec.y = (float) deltax;
            }
            if (rotateY) {
                displVec.x = (float) deltay;
            }
        }
        previousPos.x = currentPos.x;
        previousPos.y = currentPos.y;
    }

    public boolean isLeftButtonPressed() {
        return leftButtonPressed;
    }

    public boolean isRightButtonPressed() {
        return rightButtonPressed;
    }
}