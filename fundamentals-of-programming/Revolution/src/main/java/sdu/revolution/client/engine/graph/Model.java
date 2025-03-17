package sdu.revolution.client.engine.graph;

import org.joml.Matrix4f;
import sdu.revolution.client.engine.scene.Entity;

import java.util.*;

public class Model {

    private final String id;
    private final List<Animation> animationList;
    private final List<Entity> entitiesList;
    private final List<Material> materialList;

    public Model(String id, List<Material> materialList, List<Animation> animationList) {
        entitiesList = new ArrayList<>();
        this.id = id;
        this.materialList = materialList;
        this.animationList = animationList;
    }

    public void cleanup() {
        materialList.forEach(Material::cleanup);
    }

    public List<Animation> getAnimationList() {
        return animationList;
    }

    public List<Entity> getEntitiesList() {
        return entitiesList;
    }

    public String getId() {
        return id;
    }

    public List<Material> getMaterialList() {
        return materialList;
    }

    public record AnimatedFrame(Matrix4f[] boneMatrices) {
    }

    public record Animation(String name, double duration, List<AnimatedFrame> frames) {
    }
}