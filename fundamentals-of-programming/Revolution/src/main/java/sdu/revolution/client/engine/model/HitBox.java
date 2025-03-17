package sdu.revolution.client.engine.model;

import org.joml.Vector3f;

public class HitBox {

    public Vector3f center;
    public Vector3f scale;

    public HitBox(Vector3f center, Vector3f scale) {
        this.center = center;
        this.scale = scale;
    }

    public Vector3f getCenter() {
        return center;
    }

    public Vector3f getNearestSurface() {
        return new Vector3f(center.x - scale.x / 2, center.y - scale.y / 2, center.z - scale.z / 2);
    }

    public Vector3f getFarthestSurface() {
        return new Vector3f(center.x + scale.x / 2, center.y + scale.y / 2, center.z + scale.z / 2);
    }


}
