package Exercise6;

import com.alibaba.fastjson2.JSONException;
import com.alibaba.fastjson2.JSONObject;

import java.util.List;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Reader {
    public static List<Person> list = Main.manager.list;
    public static final String REGEX = ":\"\\d+,\"";

    public static void readLine(Scanner scanner) {
        if (!scanner.hasNextLine()) return;
        String s = "{\"" + scanner.nextLine().replace(",", "\",\"").replace(":", "\":\"") + "\"}";
        Matcher m = Pattern.compile(REGEX).matcher(s);
        while (m.find()) {
            s = s.replace(s.substring(m.start(), m.end()), s.substring(m.start(), m.end()).replace("\"", ""));
        }
//        System.err.println(s);
        if (s.length() <= 4) return;
        try {
            if (s.contains("salarys")) {
                list.add(JSONObject.parseObject(s, Staff.class));
            } else {
                list.add(JSONObject.parseObject(s, Student.class));
            }
        } catch (JSONException e) {
            System.err.println("FAIL: " + s);
        }
    }
}
