package sdu.revolution.client.engine.main;

import java.io.IOException;
import java.nio.file.*;

public class FileUtil {

    private FileUtil() {
        // Utility class
    }

    public static String readFile(String filePath) {
        String str;
        try {
            str = new String(Files.readAllBytes(Paths.get(filePath)));
        } catch (IOException e) {
            throw new RuntimeException("Error reading file [" + filePath + "]", e);
        }
        return str;
    }
}