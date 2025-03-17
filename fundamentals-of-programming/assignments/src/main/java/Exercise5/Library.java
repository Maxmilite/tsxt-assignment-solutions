package Exercise5;

import com.alibaba.fastjson2.JSONObject;

import java.io.*;
import java.util.Scanner;

public class Library {
    public static final String STUDENT_NOT_FOUND = "[ERROR] Student %s's information is not found.";
    public static final String CURRICULUM_NOT_FOUND = "[ERROR] Curriculum %s is not found.";
    public static final String CREDIT_REWRITTEN = "[INFO] Student %s's %s score was rewritten to %s.";
    public static final String LESSON_REWRITTEN = "[INFO] The lesson %s's name was rewritten to %s.";
    public static final String STUDENT_REWRITTEN = "[INFO] The student %s's information was rewritten.";

    public static final String CONF_PATH = System.getProperty("java.io.tmpdir").replace("\\", "/") + "StudentManager/conf.json";
    public static final String WORK_DIR = System.getProperty("java.io.tmpdir").replace("\\", "/") + "StudentManager/";

    public static void init() throws FileNotFoundException {
        System.out.println("Reading file from " + CONF_PATH);
        File file = new File(CONF_PATH);
        if (!file.exists()) {
            System.out.println("File doesn't exist.");
            if (new File(WORK_DIR).mkdir()) System.out.println("Folder was created.");
            return;
        }
        Main.manager = JSONObject.parseObject(new Scanner(new FileInputStream(CONF_PATH)).nextLine(), StudentManager.class);
        System.out.println(CONF_PATH + " was written.");
    }

    public static void exit() throws IOException {
        System.out.println("Saving file to " + CONF_PATH);
        File file = new File(CONF_PATH);
        if (!file.exists())
            if (!file.createNewFile()) throw new RuntimeException("Error while creating file " + CONF_PATH);
        new PrintStream(new FileOutputStream(CONF_PATH)).println(JSONObject.toJSONString(Main.manager));
        System.out.println(CONF_PATH + " was written.");
    }
}
