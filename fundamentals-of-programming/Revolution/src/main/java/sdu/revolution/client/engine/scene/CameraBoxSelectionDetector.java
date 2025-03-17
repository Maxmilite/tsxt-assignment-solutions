package sdu.revolution.client.engine.scene;

import org.joml.Intersectionf;
import org.joml.Vector2f;
import org.joml.Vector3f;
import sdu.revolution.client.engine.model.Item;

import java.util.List;

public class CameraBoxSelectionDetector {

    private Vector3f max;

    private Vector3f min;

    private Vector2f nearFar;
    private Vector3f dir;
    private boolean isItemSelected;
    private Item selectedItem;

    public CameraBoxSelectionDetector() {
        dir = new Vector3f();
        min = new Vector3f();
        max = new Vector3f();
        nearFar = new Vector2f();
        this.isItemSelected = false;
        this.selectedItem = null;
    }

    public void selectItem(List<Item> items, Camera camera) {
        dir = camera.getViewMatrix().positiveZ(dir).negate();
        selectItem(items, camera.getPosition(), dir);
    }

    public void selectItem(List<Item> items, Vector3f center, Vector3f dir) {
        selectedItem = null;
        float closestDistance = Float.POSITIVE_INFINITY;
        for (Item item : items) {
            item.setSelected(false);
            min.set(item.getHitbox().getNearestSurface());
            max.set(item.getHitbox().getFarthestSurface());
            if (Intersectionf.intersectRayAab(center, dir, min, max, nearFar) && nearFar.x < closestDistance) {
                closestDistance = nearFar.x;
                selectedItem = item;
            }
        }
        if (selectedItem != null) {
            selectedItem.setSelected(true);
            isItemSelected = true;
        }
    }

    public void cancelSelection(List<Item> items) {
        if (!isItemSelected)
            return;
        for (var i : items) {
            i.setSelected(false);
        }
        isItemSelected = true;
    }
}
