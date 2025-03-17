package sdu.revolution.client.engine.model;

import org.joml.Vector3f;
import sdu.revolution.client.engine.scene.Scene;
import sdu.revolution.client.engine.main.Window;
import sdu.revolution.client.engine.graph.Render;
import sdu.revolution.client.engine.scene.Entity;

public class Item {
    public boolean selected;
    public Entity entity;
    public HitBox hitbox;
    public void init(Window window, Scene scene, Render render) {}
    public void input(Window window, Scene scene, long diffTimeMillis) {}
    public void update(Window window, Scene scene, long diffTimeMillis) {}
    public Vector3f getPosition() {
        return this.entity.getPosition();
    }
    public float getScale() {
        return entity.getScale();
    }

    public HitBox getHitbox() {
        return hitbox;
    }

    public boolean isSelected() {
        return entity.isSelected();
    }

    public void setSelected(boolean selected) {
        this.selected = selected;
        entity.setSelected(selected);
    }


}
