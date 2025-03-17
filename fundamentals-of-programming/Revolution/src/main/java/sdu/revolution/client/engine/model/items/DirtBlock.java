package sdu.revolution.client.engine.model.items;

import org.joml.Vector3f;
import sdu.revolution.client.engine.graph.Model;
import sdu.revolution.client.engine.graph.Render;
import sdu.revolution.client.engine.main.Utils;
import sdu.revolution.client.engine.main.Window;
import sdu.revolution.client.engine.model.HitBox;
import sdu.revolution.client.engine.model.Item;
import sdu.revolution.client.engine.scene.Entity;
import sdu.revolution.client.engine.scene.ModelLoader;
import sdu.revolution.client.engine.scene.Scene;

public class DirtBlock extends Item {
    public Entity cubeEntity;
    public Vector3f defaultLocation;

    public void init(Window window, Scene scene, Render render) {
        Model cubeModel = ModelLoader.loadModel("cube-model" + defaultLocation, Utils.getResourceDir() + "/models/cube/dirt.obj",
                scene.getTextureCache(), false);
        scene.addModel(cubeModel);

        cubeEntity = new Entity("cube-entity" + defaultLocation, cubeModel.getId());
        cubeEntity.setPosition(defaultLocation);
        cubeEntity.updateModelMatrix();
        scene.addEntity(cubeEntity);
        super.entity = cubeEntity;
        this.hitbox = new HitBox(defaultLocation, new Vector3f(1, 1, 1));
    }
    public DirtBlock(Vector3f vec) {
        this.defaultLocation = vec;
    }
    public DirtBlock() { this(new Vector3f(0, 0, 0)); }
    public void input(Window window, Scene scene, long diffTimeMillis) { }

    public void update(Window window, Scene scene, long diffTimeMillis) {
    }
}
