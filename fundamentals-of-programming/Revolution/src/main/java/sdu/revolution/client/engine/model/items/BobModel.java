package sdu.revolution.client.engine.model.items;

import org.joml.Vector3f;
import sdu.revolution.client.engine.model.HitBox;
import sdu.revolution.client.engine.scene.AnimationData;
import sdu.revolution.client.engine.scene.ModelLoader;
import sdu.revolution.client.engine.graph.Model;
import sdu.revolution.client.engine.graph.Render;
import sdu.revolution.client.engine.main.Window;
import sdu.revolution.client.engine.model.Item;
import sdu.revolution.client.engine.scene.Entity;
import sdu.revolution.client.engine.scene.Scene;

public class BobModel extends Item {
    private AnimationData animationData;
    private Vector3f defaultLocation;
    @Override
    public void init(Window window, Scene scene, Render render) {
        super.init(window, scene, render);
        String bobModelId = "bobModel";
        Model bobModel = ModelLoader.loadModel(bobModelId, "resources/models/bob/boblamp.md5mesh", scene.getTextureCache(), true);
        scene.addModel(bobModel);
        Entity bobEntity = new Entity("bobEntity", bobModelId);
        bobEntity.setPosition(defaultLocation);
        bobEntity.setScale(0.05f);
        bobEntity.updateModelMatrix();
        animationData = new AnimationData(bobModel.getAnimationList().get(0));
        bobEntity.setAnimationData(animationData);
        scene.addEntity(bobEntity);
        super.entity = bobEntity;
        this.hitbox = new HitBox(new Vector3f(
                defaultLocation.x, defaultLocation.y + 2, defaultLocation.z),
                new Vector3f(1, 4, 1)
        );
    }

    public BobModel(Vector3f vec) {
        this.defaultLocation = vec;
    }

    @Override
    public void update(Window window, Scene scene, long diffTimeMillis) {
        super.update(window, scene, diffTimeMillis);
        animationData.nextFrame();
    }
}
