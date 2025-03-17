package io.github.maxmilite.raspberrypi;

public class Vector4B {
    public boolean up, down, left, right;
    public Vector4B() {
        this.up = false;
        this.down = false;
        this.left = false;
        this.right = false;
    }
    public Vector4B(boolean up, boolean down, boolean left, boolean right) {
        this.up = up;
        this.down = down;
        this.left = left;
        this.right = right;
    }
}
