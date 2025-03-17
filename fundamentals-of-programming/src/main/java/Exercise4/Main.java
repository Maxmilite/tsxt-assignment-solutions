package Exercise4;

import java.io.File;
import java.util.Scanner;

public class Main {

    public static void dfs(File x, int depth) {
        for (int i = 1; i <= depth; ++i)
            System.out.print("\t");
        System.out.print(x.isDirectory() ? "<d>" : "<f>");
        System.out.println(x.getName());
        if (x.isFile()) return;
        File[] child = x.listFiles();
        if (child != null) {
            for (var i : child) {
                dfs(i, depth + 1);
            }
        }
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        String path = scanner.nextLine();
        File dir = new File(path);
        dfs(dir, 0);
    }
}
