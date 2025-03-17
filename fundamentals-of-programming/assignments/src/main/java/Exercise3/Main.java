package Exercise3;

import java.util.*;

public class Main {
    public static List<Integer> list = new ArrayList<>();
    public static StringBuilder ans;

    public static void dfs(int x) {
        if (x > list.size()) return;
        dfs(x * 2);
        dfs(x * 2 + 1);
        ans.append(list.get(x - 1)).append("，");
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        ans = new StringBuilder();
        String[] input = scanner.nextLine().split("，");
        long sum = 0;
        for (var i : input) {
            sum += Integer.parseInt(i);
            list.add(Integer.parseInt(i));
        }
        sum /= input.length;
        ans.append(sum).append("#");
        List<Integer> sortList = new ArrayList<>(list);
        sortList.sort(Comparator.naturalOrder());
        for (var i : sortList) {
            ans.append(i).append("，");
        }
        ans.deleteCharAt(ans.length() - 1).append("#");
        dfs(1);
        ans.deleteCharAt(ans.length() - 1);
        System.out.println(ans);
    }
}
