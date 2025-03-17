package sdu.revolution.client.engine.main;

import com.alibaba.fastjson2.JSONObject;
import org.apache.commons.io.FileUtils;
import org.lwjgl.Version;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.charset.StandardCharsets;
import java.util.List;

public class Utils {
    public static final String NAME = "Revolution";
    public static final String VERSION = "1.0.0";
    public static final String LWJGL_VERSION = Version.getVersion();

    public static JSONObject json;

    public static String getTitle() {
        return NAME + " " + VERSION + " | Running on LWJGL " + LWJGL_VERSION;
    }
    public static String getResourceDir() { return "resources"; }

    public static float[] listFloatToArray(List<Float> list) {
        int size = list != null ? list.size() : 0;
        float[] floatArr = new float[size];
        for (int i = 0; i < size; i++) {
            floatArr[i] = list.get(i);
        }
        return floatArr;
    }

    public static int[] listIntToArray(List<Integer> list) {
        return list.stream().mapToInt((Integer v) -> v).toArray();
    }

    public static String getConfig(String key) {
        return json.get(key).toString();
    }

    public static void setConfig(String key, String value) {
        json.put(key, value);
    }

    public static void loadConfig() throws IOException {
        File dir = new File("config");
        if (!dir.exists())
            //noinspection ResultOfMethodCallIgnored
            dir.mkdirs();
        File file = new File("config/client-config.json");
        if (!file.exists()) {
            //noinspection ResultOfMethodCallIgnored
            file.createNewFile();
        }
        else {
            String val = FileUtils.readFileToString(file, StandardCharsets.UTF_8);
            json = JSONObject.parse(val);
        }
        if (json == null) {
            json = JSONObject.parse("""
                   {
                     "server-address": "localhost",
                     "server-port": "47332"
                   }
            """);
        }
    }

    public static void saveConfig() throws IOException {
        File file = new File("config/client-config.json");
        if (!file.exists()) {
            //noinspection ResultOfMethodCallIgnored
            file.createNewFile();
        }
        PrintWriter printWriter = new PrintWriter(file);
        printWriter.println(json.toJSONString());
        printWriter.flush();
        printWriter.close();
    }

}
