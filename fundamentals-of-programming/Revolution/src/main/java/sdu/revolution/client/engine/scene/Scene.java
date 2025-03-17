package sdu.revolution.client.engine.scene;

import sdu.revolution.client.engine.main.IGuiInstance;
import sdu.revolution.client.engine.graph.Model;
import sdu.revolution.client.engine.graph.TextureCache;
import sdu.revolution.client.engine.scene.lights.SceneLights;

import java.util.HashMap;
import java.util.Map;

public class Scene {
    private final Projection projection;
    private final Map<String, Model> modelMap;
    private final TextureCache textureCache;
    private final Camera camera;
    private SceneLights sceneLights;
    private SkyBox skyBox;
    private Fog fog;

    public SkyBox getSkyBox() {
        return skyBox;
    }

    public void setSkyBox(SkyBox skyBox) {
        this.skyBox = skyBox;
    }

    public Scene(int width, int height) {
        modelMap = new HashMap<>();
        projection = new Projection(width, height);
        textureCache = new TextureCache();
        camera = new Camera();
        sceneLights = new SceneLights();
        fog = new Fog();
    }

    public Fog getFog() {
        return fog;
    }

    public void setFog(Fog fog) {
        this.fog = fog;
    }

    public SceneLights getSceneLights() {
        return sceneLights;
    }

    public void setSceneLights(SceneLights sceneLights) {
        this.sceneLights = sceneLights;
    }

    public Camera getCamera() {
        return camera;
    }

    public void addEntity(Entity entity) {
        String modelId = entity.getModelId();
        Model model = modelMap.get(modelId);
        if (model == null) {
            throw new RuntimeException("Could not find model [" + modelId + "]");
        }
        model.getEntitiesList().add(entity);
    }

    public void addModel(Model model) {
        modelMap.put(model.getId(), model);
    }

    public void cleanup() {
        modelMap.values().forEach(Model::cleanup);
    }

    public Map<String, Model> getModelMap() {
        return modelMap;
    }

    public Projection getProjection() {
        return projection;
    }

    public TextureCache getTextureCache() {
        return textureCache;
    }

    public void resize(int width, int height) {
        projection.updateProjMatrix(width, height);
    }

    private IGuiInstance guiInstance;

    public IGuiInstance getGuiInstance() {
        return guiInstance;
    }

    public void setGuiInstance(IGuiInstance guiInstance) {
        this.guiInstance = guiInstance;
    }
}