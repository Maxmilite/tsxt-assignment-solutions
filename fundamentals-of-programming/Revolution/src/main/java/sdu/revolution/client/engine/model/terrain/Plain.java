package sdu.revolution.client.engine.model.terrain;

import org.joml.Vector2i;
import sdu.revolution.client.engine.model.ItemManager;

import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

public class Plain {
    public int[][] height;
    public boolean[][] vis;
    public int size;
    public static final Vector2i[] dxy = new Vector2i[] {
            new Vector2i(0, 1), new Vector2i(0, -1),
            new Vector2i(1, 0), new Vector2i(-1, 0),
    };
    private boolean checkNotOutOfBound(Vector2i vec) {
        return Math.abs(vec.x) <= size && Math.abs(vec.y) <= size;
    }
    public void cleanup() {
        height = null;
        vis = null;
    }
    private void genTerrain() {
        Queue<Vector2i> q = new LinkedList<>();
        q.offer(new Vector2i(0, 0));
        while (!q.isEmpty()) {
            Vector2i vec = q.peek();
            q.poll();
            if (!checkNotOutOfBound(vec))
                continue;
            if (vis[vec.x + size][vec.y + size])
                continue;
            vis[vec.x + size][vec.y + size] = true;
            int curHeight = height[vec.x + size][vec.y + size];
            List<Vector2i> vecList;
            if (Math.abs(vec.x) > Math.abs(vec.y)) {
                vecList = Arrays.asList(
                        new Vector2i(0, 1), new Vector2i(0, -1),
                        new Vector2i(1, 0), new Vector2i(-1, 0)
                );
            } else {
                vecList = Arrays.asList(
                        new Vector2i(1, 0), new Vector2i(-1, 0),
                        new Vector2i(0, 1), new Vector2i(0, -1)
                );
            }
            for (var i : vecList) {
                vec.add(i);
                if (!checkNotOutOfBound(vec))
                    continue;
                if (!vis[vec.x + size][vec.y + size]) {
                    double rand = Math.random();
                    if (curHeight == ItemManager.LOWEST_HEIGHT) {
                        if (rand <= 0.2) {
                            height[vec.x + size][vec.y + size] = curHeight;
                        } else {
                            height[vec.x + size][vec.y + size] = 0;
                        }
                    }
                    else if (curHeight < 0) {
                        if (rand <= 0.15) {
                            height[vec.x + size][vec.y + size] = curHeight;
                        } else if (rand <= 0.2) {
                            height[vec.x + size][vec.y + size] = curHeight - 1;
                        } else {
                            height[vec.x + size][vec.y + size] = curHeight + 1;
                        }
                    } else if (curHeight > 0) {
                        if (rand <= 0.2) {
                            height[vec.x + size][vec.y + size] = curHeight;
                        } else if (rand <= 0.3) {
                            height[vec.x + size][vec.y + size] = curHeight + 1;
                        } else {
                            height[vec.x + size][vec.y + size] = curHeight - 1;
                        }
                    } else {
                        if (rand <= 0.1) {
                            height[vec.x + size][vec.y + size] = curHeight - 1;
                        } else if (rand <= 0.2) {
                            height[vec.x + size][vec.y + size] = curHeight + 1;
                        } else {
                            height[vec.x + size][vec.y + size] = curHeight;
                        }
                    }
                    q.offer(new Vector2i(vec));
                }
                vec.sub(i);
            }
        }
    }
    public Plain(int size) {
        this.size = size;
        height = new int[size * 2 + 5][size * 2 + 5];
        vis = new boolean[size * 2 + 5][size * 2 + 5];
        height[size][size] = 0;
        genTerrain();
    }
    public int getHeight(int x, int y) {
        return height[x + size][y + size];
    }
}
